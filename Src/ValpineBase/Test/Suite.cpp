//=============================================================================|
// Copyright (C) 2015 Valpineware
// This file is licensed under the MIT License.
//=============================================================================|

#include <QFile>
#include <QtCore/QDir>
#include <QtCore/QDirIterator>
#include <QtCore/QProcess>
#include <QStandardPaths>

#include "Suite.h"
#include "private/Suite/TestMethodRunner.h"

namespace vbase { namespace test {

void Suite::run(bool launchReviewGUI, const QString &testReviewGUIPath)
{
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
	{
		qDebug() << "Full path is " << QFileInfo(ba).absoluteFilePath();

		run(ba);

		if (launchReviewGUI)
		{
			QString appPath = (testReviewGUIPath == "") ?
								  "TestReviewGUI" : testReviewGUIPath;

			QString path = QFileInfo(ba).absoluteFilePath();
			qDebug() << QProcess::startDetached(appPath, QStringList() << path);
		}
		else
		{
			qDebug() << "Not running GUI review tool";
		}
	}
}


void Suite::run(QIODevice &outputFileDevice)
{
	for (TestClassPackageInterface *testClass : registered())
	{
		_private::TestClassRunner(this, &_testResults, testClass).runAllMethods();
	}

	_testResults.exportResults(outputFileDevice);

	qDebug() << "Finished running all tests";
}


void Suite::runTestMethod(const QString &className, const QString &testName)
{
	for (TestClassPackageInterface *testClass : registered())
	{
		if (testClass->name == className)
		{
			Class *classInstance = testClass->makeTestClassInstance();

		}
	}
}


void Suite::postFailure(const QString &className, const QString &testName, Failure *failure)
{
	_testResults.findTestResult(className, testName).messages.append(failure);
}

END_NAMESPACE
END_NAMESPACE
