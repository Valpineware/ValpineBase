//=============================================================================|
// Copyright (C) 2015 Valpineware
// This file is licensed under the MIT License.
//=============================================================================|

#include <QtCore/QProcess>
#include <QtCore/QCoreApplication>
#include <QtCore/QDir>
#include <QtCore/QStandardPaths>

#include <ValpineBase/Test.h>
#include "Main.h"

int main(int argc, char *argv[])
{
	QCoreApplication app(argc, argv);

	vbase::test::Suite suite;
	suite.run(true);
	suite.testResults().cleanOldResults(60*5);
	suite.testResults().printResults();

	app.processEvents();

	return 0;
}
