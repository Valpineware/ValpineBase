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
		_private::TestClassRunner(this, &_results, testClass).runAllMethods();
	}

	_results.exportResults(outputFileDevice);

	qDebug() << "Finished running all tests";
}


QJsonObject Suite::runTestMethod(const QString &className, const QString &testName)
{
	Suite suite;
	for (TestClassPackageInterface *testClass : registered())
	{
		if (testClass->name == className)
		{
			_private::TestClassRunner testClassRunner(&suite, &suite._results, testClass);
			testClassRunner.runMethod(testName);

			QFile file(QDir::currentPath() + QString("/dump123.txt"));
			file.open(QFile::WriteOnly | QFile::Text);
			suite._results.exportResults(file);

			return suite._results.findTestResult(className, testName).toJsonObject();
		}
	}

	System::warn() << "Unable to run test method " << testName
				   << " for test class " << className;

	return QJsonObject();
}


void Suite::postFailure(const QString &className, const QString &testName, Failure *failure)
{
	qDebug() << "Posting failure " << className << " " << testName;
	_results.findTestResult(className, testName)._failures.append(Shared<Failure>(failure));
}

END_NAMESPACE
END_NAMESPACE
