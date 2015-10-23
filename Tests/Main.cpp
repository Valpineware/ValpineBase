//=============================================================================|
// Copyright (C) 2015 Valpineware
// This file is licensed under the MIT License.
//=============================================================================|

#include <QtCore/QProcess>
#include <QtCore/QCoreApplication>
#include <QtCore/QDir>
#include <QtCore/QStandardPaths>
#include <QtCore/QTextStream>
#include <QtCore/QFile>

#include <ValpineBase/Test.h>
#include "Main.h"


bool checkForIsolatedRun(const QCoreApplication &app)
{
	if (!app.arguments().contains("-runTestMethodIsolated"))
		return false;

	QString buffer;

	for (const QString &arg : app.arguments())
		buffer.append(arg + "\n");

	QFile file(QDir::currentPath() + "/dump.txt");
	file.open(QFile::WriteOnly | QFile::Text);

	QTextStream ts(&file);
	ts << buffer;

	return true;
}


void getInfo(const QCoreApplication &app)
{
	const QStringList args = app.arguments();

	auto getArgParam1 = [&args](const QString &argName)
	{
		int testClassIndex = args.indexOf("-" + argName);

		if (testClassIndex != -1 && args.count()-1 > testClassIndex)
		{
			QString testClass = args.at(testClassIndex+1);

			QFile file(QDir::currentPath() + "/" + argName + ".txt");
			file.open(QFile::WriteOnly | QFile::Text);

			QTextStream ts(&file);
			ts << testClass;
		}
	};

	getArgParam1("testClass");
	getArgParam1("testMethod");
	getArgParam1("isolatedDumpDir");
}


int main(int argc, char *argv[])
{
	QCoreApplication app(argc, argv);

	if (checkForIsolatedRun(app))
	{
		getInfo(app);

		return 0;
	}

	vbase::test::Suite suite;
	suite.run(true);
	suite.testResults().cleanOldResults(60*5);
	suite.testResults().printResults();

	app.processEvents();

	return 0;
}
