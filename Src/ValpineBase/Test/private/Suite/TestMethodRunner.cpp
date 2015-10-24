#include <QtCore/QProcess>
#include <QtCore/QCoreApplication>

#include "TestMethodRunner.h"

namespace _private {

TestClassRunner::TestClassRunner(Suite *hostSuite,
								 Results *testResults,
								 TestClassPackageInterface *testClass) :
	hostSuite(hostSuite),
	testResults(testResults),
	testClass(testClass)
{
}


void TestClassRunner::runAllMethods()
{
	testResults->setDateTimeStarted(QDateTime::currentDateTime());

	if (!isolatedDumpDir.isValid())
	{
		qFatal("Unable to create isolated dump directory!");
		return;
	}

	auto classInstance(testClass->makeTestClassInstance());
	metaObject = classInstance->metaObject();
	queryInitMethodIndex(classInstance.get());

	//run each test method for this class
	for (int i=0; i<metaObject->methodCount(); i++)
	{
		auto metaMethod = metaObject->method(i);
		QString tag = QString(metaMethod.tag());

		if (tag == "VTEST")
		{
			runMethod(metaMethod);
		}
		else if (tag == "VTEST_ISOLATED")
		{
			runMethodInSeparateProcess(metaMethod);
		}
	}

	testResults->setDateTimeFinished(QDateTime::currentDateTime());
}


void TestClassRunner::runMethod(const QString &methodName)
{
	auto classInstance(testClass->makeTestClassInstance());
	metaObject = classInstance->metaObject();
	queryInitMethodIndex(classInstance.get());

	int metaMethodIndex = classInstance->metaObject()
						  ->indexOfMethod(methodName.toStdString().c_str());

	if (metaMethodIndex != -1)
	{
		runMethod(classInstance->metaObject()->method(metaMethodIndex));
	}
}


void TestClassRunner::queryInitMethodIndex(Class *classInstance)
{
	if (initMethodIndex == -2)
	{
		metaObject = classInstance->metaObject();
		initMethodIndex = metaObject->indexOfMethod("initTestMethod()");
	}
}


void TestClassRunner::runMethod(const QMetaMethod &metaMethod)
{
	std::unique_ptr<Class> testObject(testClass->makeTestClassInstance());

	testObject->hostSuite = hostSuite;
	testObject->executionTimer.start();	//TODO why does the testObject manage its own timer?

	//run the init method if one exists
	if (initMethodIndex != -1)
		metaObject->method(initMethodIndex).invoke(testObject.get(),
												   Qt::DirectConnection);
	try
	{
		testObject->currentlyExecutingMethodName = metaMethod.name();
		metaMethod.invoke(testObject.get(), Qt::DirectConnection);


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
	auto &tr = testResults->findTestResult(metaObject->className(),
										   metaMethod.name());
	tr.executionTime = executionTime;
}


void TestClassRunner::runMethodInSeparateProcess(const QMetaMethod &metaMethod)
{	
	QStringList arguments;
	{
		arguments << "-runTestMethodIsolated";
		arguments << "-testClass" << metaObject->className();
		arguments << "-testMethod" << metaMethod.name();
		arguments << "-isolatedDumpDir" << isolatedDumpDir.path();
	}

	{
		QProcess isolatedProcess;
		QString path = QCoreApplication::instance()->arguments()[0];
		isolatedProcess.start(path, arguments);
		isolatedProcess.waitForFinished();
	}
}

END_NAMESPACE
