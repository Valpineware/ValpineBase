//=============================================================================|
// Copyright (C) 2015 Valpineware
// This file is licensed under the MIT License.
//=============================================================================|

#ifndef vbase_test_Suite_private_Suite_TestMethodRunner_h
#define vbase_test_Suite_private_Suite_TestMethodRunner_h

#include <ValpineBase/Test/Suite.h>

using namespace vbase::test;

namespace _private {

class TestClassRunner
{
public:
	TestClassRunner() = delete;
	TestClassRunner(Suite *hostSuite, Results *testResults,
					TestClassPackageInterface *testClass);

	void run();

private:
	Suite *_hostSuite = nullptr;
	Results *_testResults = nullptr;
	TestClassPackageInterface *_testClass = nullptr;
	const QMetaObject *_metaObject;
	int _initMethodIndex = -1;

	void runTestMethod(const QMetaMethod &metaMethod);
};

END_NAMESPACE

#endif
