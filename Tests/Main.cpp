//=============================================================================|
// Copyright (C) 2015 Valpineware
// This file is licensed under the MIT License.
//=============================================================================|

#include <QtCore/QProcess>
#include <QtCore/QCoreApplication>
#include <QtCore/QDir>
#include <QtCore/QStandardPaths>
#include <QtCore/QTextStream>
#include <QtCore/QFile>
#include <QtCore/QJsonDocument>

#include <ValpineBase/Test.h>
#include "Main.h"

struct
{
	QString testClass;
	QString testMethod;
	QString isolatedDumpDir;
} isolatedInfo;


bool checkForIsolatedRun(const QCoreApplication &app)
{
	if (!app.arguments().contains("-runTestMethodIsolated"))
		return false;

	QString buffer;

	for (const QString &arg : app.arguments())
		buffer.append(arg + "\n");

	QFile file(QDir::currentPath() + "/dump.txt");
	file.open(QFile::WriteOnly | QFile::Text);

	QTextStream ts(&file);
	ts << buffer;

	return true;
}


void getInfo(const QCoreApplication &app)
{
	const QStringList args = app.arguments();

	auto getArgParam1 = [&args](const QString &argName) -> QString
	{
		int testClassIndex = args.indexOf("-" + argName);

		if (testClassIndex != -1 && args.count()-1 > testClassIndex)
			return args.at(testClassIndex+1);

		return "";
	};

	isolatedInfo.testClass = getArgParam1("testClass");
	isolatedInfo.testMethod = getArgParam1("testMethod");
	isolatedInfo.isolatedDumpDir = getArgParam1("isolatedDumpDir");
}


void runIsolatedTestMethod()
{
	vbase::test::Suite suite;
	auto jsonObject = suite.runTestMethod(isolatedInfo.testClass, isolatedInfo.testMethod);

	QJsonDocument jsonDocument(jsonObject);

	QFile file(QDir::currentPath() + "/jsonDump.txt");
	file.open(QFile::WriteOnly | QFile::Text);

	QTextStream ts(&file);
	ts << jsonDocument.toJson();
}


int main(int argc, char *argv[])
{
	QCoreApplication app(argc, argv);

	if (checkForIsolatedRun(app))
	{
		getInfo(app);
		runIsolatedTestMethod();

		return 0;
	}

	vbase::test::Suite suite;
	suite.run(true);
	suite.testResults().cleanOldResults(60*5);
	suite.testResults().printResults();

	app.processEvents();

	return 0;
}
