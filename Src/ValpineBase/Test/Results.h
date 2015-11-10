//=============================================================================|
// Copyright (C) 2015 Valpineware
// This file is licensed under the MIT License.
//=============================================================================|

#ifndef vbase_test_Results_h
#define vbase_test_Results_h

#include <QtCore/QIODevice>

#include <ValpineBase/Test/Failure.h>

namespace vbase { namespace test {

class Suite;

/**
 * Stores test results organized per test class by name. Used as a result from
 * running a Suite.
 */
class Results
{
public:
	/**
	 * Stores information about a test method that was executed.
	 */
	struct TestResult
	{
		TestResult() = default;

		explicit TestResult(const QJsonObject &jsonObject)
		{
			fromJsonObject(jsonObject);
		}

		/**
		 * @brief The name of the test method.
		 */
		QString _name;

		/**
		 * @brief The amount of time in milliseconds this test took to execute.
		 * Includes the time to call initTestCase.
		 */
		int _executionTime = 0;

		/**
		 * @brief List of all failures posted while running the test.
		 */

		QList<Shared<Failure>> _failures;

		/**
		 * @return the status string which is based on the types of failures. If
		 * at least one message is type Error, "error" is returned. Otherwise
		 * if at least one message is type Warn, "warning" is returned.
		 * Otherwise "passed" is returned.
		 */
		QString status() const;

		QJsonObject toJsonObject() const;

	private:
		void fromJsonObject(const QJsonObject &jsonObject);
	};


	struct ClassResult
	{
		ClassResult() = default;

		ClassResult(const QJsonObject &jsonObject)
		{
			fromJsonObject(jsonObject);
		}

		QList<TestResult> _testResults;

		//TODO remove the need to pass in the key for the name
		QJsonObject toJsonObject(const QString &key) const;

	private:
		void fromJsonObject(const QJsonObject &jsonObject);
	};

	TestResult& findTestResult(const QString &className, const QString &testName);

	ClassResult& findClassResult(const QString &className);

	void exportResults(QIODevice &outDevice);

	void printResults() const;

	void cleanOldResults(int maxAgeSeconds) const;

	static const QString& dateFormat()
	{
		static QString fmt = "yyyy-MM-dd HH:mm:ss:zzz";
		return fmt;
	}

	void setDateTimeStarted(const QDateTime &dateTimeStarted)
	{
		_dateTimeStarted = dateTimeStarted;
	}

	void setDateTimeFinished(const QDateTime &dateTimeFinished)
	{
		_dateTimeStarted = dateTimeFinished;
	}

private:
	QMap<QString, ClassResult> _results;
	QDateTime _dateTimeStarted;
	QDateTime _dateTimeFinished;
};

END_NAMESPACE
END_NAMESPACE

#endif
