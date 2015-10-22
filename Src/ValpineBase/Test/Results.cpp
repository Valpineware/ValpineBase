#include <QtCore/QDirIterator>
#include <QtCore/QFileInfo>
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>
#include <QtCore/QJsonArray>
#include <QtCore/QJsonValue>

#include <ValpineBase/Test/Suite.h>

#include "Results.h"

namespace vbase { namespace test {

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


void Results::exportResults(QIODevice &outDevice)
{
	QJsonObject rootJson;
	rootJson.insert("dateTime_started", _dateTime_started.toString(dateFormat()));
	rootJson.insert("dateTime_finished", _dateTime_finished.toString(dateFormat()));

	QJsonArray classesJsonArray;

	//build array of results
	QMapIterator<QString,QList<TestResult>> iter = _results;

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
	QTextStream out(&outDevice);
	out << doc.toJson();
}


Results::TestResult& Results::findTestResult(const QString&className,
											 const QString&testName)
{
	auto classIter = _results.find(className);

	//an entry for this className already exists
	if (classIter != _results.end())
	{
		for (auto &testResult : classIter.value())
			if (testResult.name == testName)
				return testResult;

		classIter.value().append(TestResult());
		classIter.value().last().name = testName;

		return classIter.value().last();
	}

	auto &testResultList = _results[className];
	testResultList = QList<TestResult>();
	testResultList.append(TestResult());
	testResultList.last().name  = testName;

	return testResultList.first();
}


void Results::printResults() const
{
	QMapIterator<QString, QList<TestResult>> iter(_results);

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


void Results::cleanOldResults(int maxAgeSeconds) const
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


QString Results::TestResult::status() const
{
	bool foundWarning = false;

	for (Failure *message : messages)
	{
		if (message->type == Failure::Type::Error)
			return "error";
		else if (message->type == Failure::Type::Warn)
			foundWarning = true;
	}

	if (foundWarning)
		return "warning";

	return "passed";
}

END_NAMESPACE
END_NAMESPACE
