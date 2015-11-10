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
		for (TestResult &testResult : classIter.value()._testResults)
			if (testResult._name == testName)
				return testResult;

		classIter.value()._testResults.append(TestResult());
		classIter.value()._testResults.last()._name = testName;

		return classIter.value()._testResults.last();
	}

	auto &testResultList = _results[className]._testResults;
	testResultList.clear();
	testResultList.append(TestResult());
	testResultList.last()._name  = testName;

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
		auto classResultIter = iter.next();

		for (const TestResult &testResult : classResultIter.value()._testResults)
		{
			for (auto failure : testResult._failures)
			{
				qDebug() << "FAILED: [" << testResult._name << "] - - - - - - - -";

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
	QDirIterator dirIter("./TestResults", QDirIterator::Subdirectories);

	while (dirIter.hasNext())
	{
		QFileInfo fi(dirIter.next());
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

	for (const Shared<Failure> &failure : _failures)
	{
		if (failure->type == Failure::Type::Error)
			return "error";
		else if (failure->type == Failure::Type::Warn)
			foundWarning = true;
	}

	if (foundWarning)
		return "warning";

	return "passed";
}


QJsonObject Results::TestResult::toJsonObject() const
{
	QJsonObject jsonObject;
	jsonObject.insert("name", _name);
	jsonObject.insert("status", status());
	jsonObject.insert("executionTime", _executionTime);

	QJsonArray resultsArray;

	for (const Shared<Failure> &failure : _failures)
	{
		resultsArray.append(failure->toJsonObject());
	}

	jsonObject.insert("messages", resultsArray);

	return jsonObject;
}


void Results::TestResult::fromJsonObject(const QJsonObject &jsonObject)
{
	_name = jsonObject["name"].toString();
	_executionTime = jsonObject["executionTime"].toInt();

	for (const auto &failureJsonObject : jsonObject["messages"].toArray())
	{
		_failures.append(Shared<Failure>(new Failure(failureJsonObject.toObject())));
	}
}


QJsonObject Results::ClassResult::toJsonObject(const QString &key) const
{
	QJsonObject jsonObject;
	jsonObject.insert("className", QJsonValue(key));

	QJsonArray testsArray;

	//TODO fix all of these magic strings
	for (const TestResult &testResult : _testResults)
		testsArray.append(testResult.toJsonObject());

	jsonObject.insert("tests", testsArray);

	return jsonObject;
}


void Results::ClassResult::fromJsonObject(const QJsonObject &jsonObject)
{
	//TODO wtf now?
	QString name = jsonObject["className"].toString();

	for (const auto &testResultJsonValue : jsonObject["tests"].toArray())
		_testResults.append(TestResult(testResultJsonValue.toObject()));
}

END_NAMESPACE
END_NAMESPACE
