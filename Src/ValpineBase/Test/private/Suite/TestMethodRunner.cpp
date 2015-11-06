//=============================================================================|
// Copyright (C) 2015 Valpineware
// This file is licensed under the MIT License.
//=============================================================================|

#include <QtCore/QProcess>
#include <QtCore/QCoreApplication>
#include <QtCore/QJsonDocument>
#include <QtCore/QTemporaryFile>
#include <QtCore/QFileInfo>

#include "TestMethodRunner.h"

namespace _private {

TestClassRunner::TestClassRunner(Suite *hostSuite,
								 Results *testResults,
								 TestClassPackageInterface *testClass) :
	_hostSuite(hostSuite),
	_testResults(testResults),
	_testClass(testClass)
{
}

void TestClassRunner::runAllMethods()
{
	_testResults->setDateTimeStarted(QDateTime::currentDateTime());

	auto classInstance(_testClass->makeTestClassInstance());
	_metaObject = classInstance->metaObject();
	queryInitMethodIndex(classInstance.get());

	//run each test method for this class
	for (int i=0; i<_metaObject->methodCount(); i++)
	{
		auto metaMethod = _metaObject->method(i);
		QStringList tags = QString(metaMethod.tag()).split(" ");

		if (tags.contains("VTEST"))
		{
			runMethod(metaMethod);
		}
		else if (tags.contains("VTEST_ISOLATED"))
		{
			runMethodInSeparateProcess(metaMethod, extractTimeTagValue(tags));
		}
	}

	_testResults->setDateTimeFinished(QDateTime::currentDateTime());
}


void TestClassRunner::runMethod(const QString &methodName)
{
	auto classInstance(_testClass->makeTestClassInstance());
	_metaObject = classInstance->metaObject();
	queryInitMethodIndex(classInstance.get());

	QString normalizeMethodName = methodName + "()";
	int metaMethodIndex = _metaObject->indexOfMethod(normalizeMethodName.toStdString().c_str());

	if (metaMethodIndex != -1)
	{
		runMethod(classInstance->metaObject()->method(metaMethodIndex));
	}
}


void TestClassRunner::queryInitMethodIndex(Class *classInstance)
{
	if (_initMethodIndex == -2)
	{
		_metaObject = classInstance->metaObject();
		_initMethodIndex = _metaObject->indexOfMethod("initTestMethod()");
	}
}


void TestClassRunner::runMethod(const QMetaMethod &metaMethod)
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
	auto &tr = _testResults->findTestResult(_metaObject->className(),
										   metaMethod.name());
	tr.executionTime = executionTime;
}


void TestClassRunner::runMethodInSeparateProcess(const QMetaMethod &metaMethod,
												 int timeoutSeconds)
{	
	QTemporaryFile isolatedDumpFile;
	isolatedDumpFile.open();

	if (!isolatedDumpFile.isOpen())
	{
		qFatal("Unable to create isolated dump file!");
		return;
	}

	QStringList arguments;
	{
		arguments << "-runTestMethodIsolated";
		arguments << "-testClass" << _metaObject->className();
		arguments << "-testMethod" << metaMethod.name();
		arguments << "-isolatedDumpFile"
					 << QFileInfo(isolatedDumpFile).absoluteFilePath();
	}

	//run the process
	{
		QProcess isolatedProcess;
		QString path = QCoreApplication::instance()->arguments()[0];
		isolatedProcess.start(path, arguments);
		isolatedProcess.waitForFinished(timeoutSeconds * 1000);

		//if the test timed out, we don't use the Json dump file because the
		//process was forcibly terminated which means the file could be corrupt
		if (isolatedProcess.error() == QProcess::Timedout)
		{
			auto failure = new Failure;
			failure->type = Failure::Type::Error;
			QString msg = QString("The test timed out after " ) +
						  QString::number(timeoutSeconds) + " seconds.";
			failure->details.append(msg);

			_hostSuite->postFailure(QString(_metaObject->className()),
								   metaMethod.name(), failure);
		}
		//use the Json dump file
		else
		{
			QTextStream ts(&isolatedDumpFile);
			QJsonParseError jsonParseError;
			QByteArray buffer = ts.readAll().toLocal8Bit();
			auto jsonDocument = QJsonDocument::fromJson(buffer, &jsonParseError);

			if (jsonParseError.error != QJsonParseError::NoError)
			{
				qDebug() << "Parse error: " << jsonParseError.errorString();
			}
			else
			{
				auto &classResult = _testResults->findClassResult(_metaObject->className());
				auto testResult = Results::TestResult(jsonDocument.object());
				classResult.testResults.append(testResult);
			}
		}
	}
}


int TestClassRunner::extractTimeTagValue(const QStringList &tags) const
{
	static std::map<QString,int> lk
	{
		{ "VTIME_1", 1 },
		{ "VTIME_5", 5 },
		{ "VTIME_10", 10 },
		{ "VTIME_30", 30 },
		{ "VTIME_60", 60 },
		{ "VTIME_300", 300 },
		{ "VTIME_600", 600 }
	};

	for (const auto &timeTag : lk)
	{
		if (tags.contains(timeTag.first))
			return timeTag.second;
	}

	return _defaultIsolatedTestTimeout;
}

END_NAMESPACE
