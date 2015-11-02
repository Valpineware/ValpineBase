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
	TestClassRunner(Suite *_hostSuite, Results *_testResults,
					TestClassPackageInterface *_testClass);

	void runAllMethods();

	void runMethod(const QString &methodName);

private:
	Suite *_hostSuite = nullptr;
	Results *_testResults = nullptr;
	TestClassPackageInterface *_testClass = nullptr;
	const QMetaObject *_metaObject;
	int _initMethodIndex = -2;

	void queryInitMethodIndex(Class *classInstance);
	void runMethod(const QMetaMethod &metaMethod);
	void runMethodInSeparateProcess(const QMetaMethod &metaMethod,
									int timeoutSeconds);
	int extractTimeTagValue(const QStringList &tags) const;
};

END_NAMESPACE

#endif
