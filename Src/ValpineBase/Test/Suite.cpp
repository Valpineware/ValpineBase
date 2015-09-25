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
					testObject->executionTimer.start();
					metaMethod.invoke(testObject.get(), Qt::DirectConnection);
					int executionTime = testObject->executionTimer.elapsed();

					//at this point, the test must have passed since no
					//exception was thrown

					auto &tr = findTestResult(metaObject->className(), metaMethod.name());

					auto result = new Result;
					result->executionTime = executionTime;
					tr.results.append(result);
				}
				catch (TestAssertException &tfe)
				{
					//swallow the exception
					//the purpose of throwing the exception from an assert
					//is to cleanly break out of the test entirely
					//(even from sub-routines)

					//WARNING ensure the ResultFailure* is posted from the assert statement
					//testResult.results.append(tfe.pResultFailure);
				}
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
			for (Result *result : testResult.results)
			{
				if (auto failure = dynamic_cast<ResultFailure*>(result))
				{
					qDebug() << "FAILED: [" << testResult.name << "] - - - - - - - -";

					for (auto line : failure->message)
					{
						qDebug() << "      -" << line;
					}

					qDebug() << "  At " << failure->filepath;
					qDebug() << "  Line " << failure->lineNumber;

					qDebug() << "";
				}
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

		if (fi.lastModified().secsTo(QDateTime::currentDateTime())
				> maxAgeSeconds)
		{
			QFile::remove(fi.absoluteFilePath());
		}
	}
}


void Suite::post(const QString &className, const QString &testName, Result *result)
{
	findTestResult(className, testName).results.append(result);
}


QJsonObject jsonObjectFromResult(const Result *result)
{
	QJsonObject o;
	o.insert("executionTime", QString::number(result->executionTime));

	if (auto *p = dynamic_cast<const ResultFailure*>(result))
	{
		o.insert("status", QString("failed"));
		o.insert("filePath", p->filepath); //TODO fix filepath to filePath
		o.insert("lineNumber", p->lineNumber);

		QJsonArray messageArray;
		for (const auto &message : p->message)
			messageArray.append(message);

		o.insert("message", messageArray);
	}
	else
	{
		o.insert("status", QJsonValue("passed"));
	}

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

		for (const TestResult &testResult : a.value())
		{
			QJsonObject to;
			to.insert("name", testResult.name);

			QJsonArray resultsArray;

			for (const Result *result : testResult.results)
			{
				resultsArray.append(jsonObjectFromResult(result));
			}

			to.insert("results", resultsArray);
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

	return testResultList.first();
}




}}
