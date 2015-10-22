//=============================================================================|
// Copyright (C) 2015 Valpineware
// This file is licensed under the MIT License.
//=============================================================================|

#ifndef vbase_test_Suite_h
#define vbase_test_Suite_h

#include <QtCore/QObject>
#include <QtCore/QDebug>
#include <QtCore/QString>
#include <QtCore/QList>
#include <QtCore/QMap>
#include <QtCore/QMetaObject>
#include <QtCore/QMetaMethod>
#include <QtCore/QIODevice>
#include <QtCore/QDateTime>

#include <ValpineBase/ValpineBase.h>
#include <ValpineBase/System.h>
#include <ValpineBase/Test/Failure.h>
#include <ValpineBase/Test/Class.h>

#include "private/Suite/TestResults.h"

namespace vbase { namespace test {

class TestClassPackageInterface
{
public:
	virtual Class *makeTestClassInstance() = 0;
};


// TODO convert to private implementation
template<typename T>
class TestClassPackage : public TestClassPackageInterface
{
public:
	virtual Class *makeTestClassInstance()
	{
		return new T;
	}
};


class Suite
{
public:
	/**
	 * Runs all the tests registered with the Suite. The test results are
	 * written to ./TestResults/ and are uniquely identified with a timestamp.
	 *
	 * If \p launchReviewGUI is true, then the TestReviewGUI application will
	 * be launched with an argument of the path to the test results JSON file.
	 *
	 * \p testReviewGUIPath points to the TestReviewGUI executable which
	 * should be launched. If \testReveiwGUIPath is blank, then it is
	 * assumed that "TestReviewGUI" is a recognized command in the system
	 * PATH and will be invoked as such.
	 */
	void run(bool launchReviewGUI, const QString &testReviewGUIPath="");

	/**
	 * Runs all the tests registered with the Suite. Test results are
	 * written to the \outputFileDevice as JSON text.
	 */
	void run(QIODevice &outputFileDevice);

	void postFailure(const QString &className, const QString &testName, Failure *result);

	const TestResults& testResults() const { return _testResults; }

	template<typename T>
	struct TestAdder
	{
		TestAdder()
		{
			registered().append(new TestClassPackage<T>);
		}
	};

private:
	static QList<TestClassPackageInterface*>& registered()
	{
		static QList<TestClassPackageInterface*> reg;
		return reg;
	}

	TestResults _testResults;
};

#ifndef Q_MOC_RUN

//tags a method as a regular test method
#define VTEST

//tags a method as a test method to be run in an isolated process
#define VTEST_ISOLATED

#endif

#define ADD_TESTCLASS(name) \
	static vbase::test::Suite::TestAdder<name> t_##name;

END_NAMESPACE
END_NAMESPACE

#endif
