//=============================================================================|
// Copyright (C) 2015 Valpineware
// This file is licensed under the MIT License.
//=============================================================================|

#include <QtCore/QProcess>
#include <QtCore/QCoreApplication>
#include <QtCore/QDir>
#include <QtCore/QStandardPaths>

#include <ValpineBase/Test.h>
#include "Main.h"

int main(int argc, char *argv[])
{
	QCoreApplication app(argc, argv);

	vbase::test::Suite suite;

	QString filepath = QDir::currentPath() + QString("/TestResults/");
	QDir().mkpath(filepath);

	QFile ba(filepath
			 + "Result_"
			 + QDateTime::currentDateTime().toString("yyyyMMddHHmmsszzz")
			 + ".json");

	if (!ba.open(QFile::WriteOnly | QFile::Text))
	{
		qDebug() << "Unable to write test output";
		qDebug() << filepath;
		qDebug() << ba.errorString();
	}

	else
		suite.run(ba);

	app.processEvents();

	QString path = QFileInfo(ba).absoluteFilePath();
	qDebug() << QProcess::startDetached("TestReviewGUI", QStringList() << path);

	return 0;
}
