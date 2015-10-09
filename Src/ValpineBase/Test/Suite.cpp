//=============================================================================|
// Copyright (C) 2015 Valpineware
// This file is licensed under the MIT License.
//=============================================================================|

#include <QFile>
#include <QtCore/QDir>
#include <QtCore/QDirIterator>
#include <QtCore/QProcess>
#include <QStandardPaths>

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>

#include "Suite.h"

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
	mDateTime_started = QDateTime::currentDateTime();

	for (TestClassPackageInterface *test : registered())
	{
		std::unique_ptr<Class> defaultInstance(test->makeTestClassInstance());
		const QMetaObject *metaObject = defaultInstance->metaObject();

		for (int i=0; i<metaObject->methodCount(); i++)
		{
			auto metaMethod = metaObject->method(i);

			if (QString(metaMethod.tag()) == "VTEST")
			{
				std::unique_ptr<Class> testObject(test->makeTestClassInstance());
				testObject->hostSuite = this;

				try
				{
					testObject->currentlyExecutingMethodName = metaMethod.name();
					testObject->executionTimer.start();
					metaMethod.invoke(testObject.get(), Qt::DirectConnection);


					//at this point, the test must have passed since no
					//exception was thrown
				}
				catch (TestAssertException &tfe)
				{
					//swallow the exception
					//the purpose of throwing the exception from an assert
					//is to cleanly break out of the test entirely
					//(even from sub-routines)
				}

				int executionTime = testObject->executionTimer.elapsed();
				auto &tr = findTestResult(metaObject->className(),
										  metaMethod.name());
				tr.executionTime = executionTime;
			}
		}
	}

	mDateTime_finished = QDateTime::currentDateTime();

	exportResults(outputFileDevice);

	qDebug() << "Finished running all tests";
}


void Suite::printResults()
{
	QMapIterator<QString, QList<TestResult>> iter(mResults);
	while (iter.hasNext())
	{
		auto item = iter.next();

		for (const TestResult &testResult : item.value())
		{
			for (Failure *failure : testResult.messages)
			{
				qDebug() << "FAILED: [" << testResult.name << "] - - - - - - - -";

				for (auto line : failure->details)
				{
					qDebug() << "      -" << line;
				}

				qDebug() << "  At " << failure->filePath;
				qDebug() << "  Line " << failure->lineNumber;

				qDebug() << "";
			}
		}
	}
}


void Suite::cleanOldResults(int maxAgeSeconds)
{
	QDirIterator iter("./TestResults", QDirIterator::Subdirectories);
	while (iter.hasNext())
	{
		QFileInfo fi(iter.next());
		int testAgeSec = fi.lastModified().secsTo(QDateTime::currentDateTime());

		if (testAgeSec > maxAgeSeconds)
		{
			QFile::remove(fi.absoluteFilePath());
		}
	}
}


void Suite::postFailure(const QString &className, const QString &testName, Failure *failure)
{
	findTestResult(className, testName).messages.append(failure);
}


QJsonObject jsonObjectFromResult(const Failure *failure)
{
	QJsonObject o;
	o.insert("filePath", failure->filePath);
	o.insert("lineNumber", failure->lineNumber);
	o.insert("type", static_cast<int>(failure->type));

	QJsonArray messageArray;
	for (const auto &message : failure->details)
		messageArray.append(message);

	o.insert("details", messageArray);

	return o;
}


void Suite::exportResults(QIODevice &ioDevice)
{
	QJsonObject rootJson;
	rootJson.insert("dateTime_started", mDateTime_started.toString(dateFormat()));
	rootJson.insert("dateTime_finished", mDateTime_finished.toString(dateFormat()));

	QJsonArray classesJsonArray;

	//build array of results
	QMapIterator<QString,QList<TestResult>> iter = mResults;

	while (iter.hasNext())
	{
		auto a = iter.next();

		QJsonObject co;
		co.insert("className", QJsonValue(a.key()));

		QJsonArray testsArray;

		//TODO fix all of these magic strings
		for (const TestResult &testResult : a.value())
		{
			QJsonObject to;
			to.insert("name", testResult.name);
			to.insert("status", testResult.status());
			to.insert("executionTime", testResult.executionTime);

			QJsonArray resultsArray;

			for (const Failure *failure : testResult.messages)
			{
				resultsArray.append(jsonObjectFromResult(failure));
			}

			to.insert("messages", resultsArray);
			testsArray.append(to);
		}

		co.insert("tests", testsArray);
		classesJsonArray.append(co);
	}

	rootJson.insert("results", classesJsonArray);

	QJsonDocument doc(rootJson);
	QTextStream out(&ioDevice);
	out << doc.toJson();
}


Suite::TestResult& Suite::findTestResult(const QString &className, const QString &testName)
{
	auto classIter = mResults.find(className);

	//an entry for this className already exists
	if (classIter != mResults.end())
	{
		for (auto &testResult : classIter.value())
			if (testResult.name == testName)
				return testResult;

		classIter.value().append(TestResult());
		classIter.value().last().name = testName;

		return classIter.value().last();
	}

	auto &testResultList = mResults[className];
	testResultList = QList<TestResult>();
	testResultList.append(TestResult());
	testResultList.last().name  = testName;

	return testResultList.first();
}

END_NAMESPACE
END_NAMESPACE
