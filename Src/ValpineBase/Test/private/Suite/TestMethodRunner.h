//=============================================================================|
// Copyright (C) 2015 Valpineware
// This file is licensed under the MIT License.
//=============================================================================|

#ifndef vbase_test_Suite_private_Suite_TestMethodRunner_h
#define vbase_test_Suite_private_Suite_TestMethodRunner_h

#include <QtCore/QTemporaryDir>

#include <ValpineBase/Test/Suite.h>

using namespace vbase::test;

namespace _private {

class TestClassRunner
{
public:
	TestClassRunner() = delete;
	TestClassRunner(Suite *hostSuite, Results *testResults,
					TestClassPackageInterface *testClass);

	void runAllMethods();

	void runMethod(const QString &methodName);

private:
	Suite *hostSuite = nullptr;
	Results *testResults = nullptr;
	TestClassPackageInterface *testClass = nullptr;
	const QMetaObject *metaObject;
	int initMethodIndex = -2;
	QTemporaryDir isolatedDumpDir;

	void queryInitMethodIndex(Class *classInstance);
	void runMethod(const QMetaMethod &metaMethod);
	void runMethodInSeparateProcess(const QMetaMethod &metaMethod);
};

END_NAMESPACE

#endif
