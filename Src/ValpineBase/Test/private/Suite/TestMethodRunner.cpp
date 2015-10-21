#include "TestMethodRunner.h"

namespace _Private {

TestClassRunner::TestClassRunner(Suite *hostSuite,
								 TestClassPackageInterface *testClass) :
	_testClass(testClass)
{
}


void TestClassRunner::run()
{
	std::unique_ptr<Class> defaultInstance(_testClass->makeTestClassInstance());
	_metaObject = defaultInstance->metaObject();
	_initMethodIndex = _metaObject->indexOfMethod("initTestMethod()");

	//run each test method for this class
	for (int i=0; i<_metaObject->methodCount(); i++)
	{
		auto metaMethod = _metaObject->method(i);

		if (QString(metaMethod.tag()) == "VTEST")
		{
			runTestMethod(metaMethod);
		}
	}
}


void TestClassRunner::runTestMethod(const QMetaMethod &metaMethod)
{
	std::unique_ptr<Class> testObject(_testClass->makeTestClassInstance());

	testObject->hostSuite = _hostSuite;
	testObject->executionTimer.start();	//TODO why does the testObject manage its own timer?

	//run the init method if one exists
	if (_initMethodIndex != -1)
		_metaObject->method(_initMethodIndex).invoke(testObject.get(),
												   Qt::DirectConnection);
	try
	{
		testObject->currentlyExecutingMethodName = metaMethod.name();
		_metaObject->invoke(testObject.get(), Qt::DirectConnection);


		//at this point, the test must have passed since no
		//exception was thrown
	}
	catch (TestAssertException &tfe)
	{
		//swallow the exception
		//the purpose of throwing the exception from an assert
		//is to cleanly break out of the test entirely
		//(even from sub-routines)
	}

	int executionTime = testObject->executionTimer.elapsed();
	auto &tr = findTestResult(_metaObject->className(),
							  metaMethod.name());
	tr.executionTime = executionTime;
}

END_NAMESPACE
