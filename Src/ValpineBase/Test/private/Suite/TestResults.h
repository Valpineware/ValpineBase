//=============================================================================|
// Copyright (C) 2015 Valpineware
// This file is licensed under the MIT License.
//=============================================================================|

#ifndef vbase_test_Suite_private_Suite_TestResults_h
#define vbase_test_Suite_private_Suite_TestResults_h

#include <ValpineBase/ValpineBase.h>

namespace vbase { namespace test {

class Suite;

//TODO consider making this a public class
class TestResults
{
public:
	struct TestResult
	{
		QString name;
		int executionTime = 0;
		QList<Failure*> messages;

		/**
		 * @return the status string which is based on the types of messages. If
		 * at least one message is type Error, "error" is returned. Otherwise
		 * if at least one message is type Warn, "warning" is returned.
		 * Otherwise "passed" is returned.
		 */
		QString status() const;
	};

	void exportResults(QIODevice &ioDevice);

	TestResult& findTestResult(const QString &className, const QString &testName);
	void printResults() const;

	void cleanOldResults(int maxAgeSeconds) const;

	static const QString& dateFormat()
	{
		static QString fmt = "yyyy-MM-dd HH:mm:ss:zzz";
		return fmt;
	}

	QMap<QString, QList<TestResult>> _results;
	QDateTime _dateTime_started, _dateTime_finished;
};

END_NAMESPACE
END_NAMESPACE

#endif
