#include <QtCore/QDirIterator>
#include <QtCore/QFileInfo>
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>
#include <QtCore/QJsonArray>
#include <QtCore/QJsonValue>

#include <ValpineBase/Test/Suite.h>

#include "Results.h"

namespace vbase { namespace test {

void Results::exportResults(QIODevice &outDevice)
{
	QJsonObject rootJson;
	rootJson.insert("dateTime_started", _dateTimeStarted.toString(dateFormat()));
	rootJson.insert("dateTime_finished", _dateTimeFinished.toString(dateFormat()));

	QJsonArray classesJsonArray;

	//build array of results
	QMapIterator<QString, ClassResult> iter = _results;

	while (iter.hasNext())
	{
		auto a = iter.next();
		classesJsonArray.append(a.value().toJsonObject(a.key()));
	}

	rootJson.insert("results", classesJsonArray);

	QJsonDocument doc(rootJson);
	QTextStream out(&outDevice);
	out << doc.toJson();
}


Results::TestResult& Results::findTestResult(const QString &className,
											 const QString &testName)
{
	auto classIter = _results.find(className);

	//an entry for this className already exists
	if (classIter != _results.end())
	{
		for (auto &testResult : classIter.value().testResults)
			if (testResult.name == testName)
				return testResult;

		classIter.value().testResults.append(TestResult());
		classIter.value().testResults.last().name = testName;

		return classIter.value().testResults.last();
	}

	auto &testResultList = _results[className].testResults;
	testResultList.clear();
	testResultList.append(TestResult());
	testResultList.last().name  = testName;

	return testResultList.first();
}


Results::ClassResult& Results::findClassResult(const QString &className)
{
	return _results[className];
}


void Results::printResults() const
{
	QMapIterator<QString, ClassResult> iter(_results);

	while (iter.hasNext())
	{
		auto item = iter.next();

		for (const TestResult &testResult : item.value().testResults)
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


QJsonObject Results::TestResult::toJsonObject() const
{
	QJsonObject jsonObject;
	jsonObject.insert("name", name);
	jsonObject.insert("status", status());
	jsonObject.insert("executionTime", executionTime);

	QJsonArray resultsArray;

	for (const Failure *failure : messages)
	{
		resultsArray.append(failure->toJsonObject());
	}

	jsonObject.insert("messages", resultsArray);

	return jsonObject;
}


void Results::TestResult::fromJsonObject(const QJsonObject &jsonObject)
{
	name = jsonObject["name"].toString();
	executionTime = jsonObject["executionTime"].toInt();

	for (const auto &failureJsonObject : jsonObject["messages"].toArray())
	{
		messages.append(new Failure(failureJsonObject.toObject()));
	}
}


QJsonObject Results::ClassResult::toJsonObject(const QString &key) const
{
	QJsonObject jsonObject;
	jsonObject.insert("className", QJsonValue(key));

	QJsonArray testsArray;

	//TODO fix all of these magic strings
	for (const TestResult &testResult : testResults)
		testsArray.append(testResult.toJsonObject());

	jsonObject.insert("tests", testsArray);

	return jsonObject;
}


void Results::ClassResult::fromJsonObject(const QJsonObject &jsonObject)
{
	//TODO wtf now?
	QString name = jsonObject["className"].toString();

	for (const auto &testResultJsonValue : jsonObject["tests"].toArray())
		testResults.append(TestResult(testResultJsonValue.toObject()));
}

END_NAMESPACE
END_NAMESPACE
