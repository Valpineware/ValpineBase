//=============================================================================|
// Copyright (C) 2015 Valpineware
// This file is licensed under the MIT License.
//=============================================================================|

#include <ValpineBase/Test.h>
#include <ValpineBase/Settings.h>

#include <QtCore/QDateTime>
#include <QtCore/QTemporaryFile>

using namespace vbase;

class Test_Settings : public test::Class
{
	Q_OBJECT

private slots:
	VTEST void simpleCheck()
	{
		using Key = QString;
		const Key kWindowWidth = "Window/Width";
		const Key kWindowHeight = "Window/Height";
		const Key kWindowIsFullscreen = "Window/IsFullScreen";

		Settings settings;
		//TODO get a better way to generate an isolated directory for files
		Assert_True(settings.load("MyHackFileFixLater.ini"));

		settings.setValue(kWindowWidth, 800);
		settings.setValue(kWindowHeight, 600);
		settings.setValue(kWindowIsFullscreen, false);

		qDebug() << "Here: " << QVariant(QDate(1995, 1, 3)).toString();
		qDebug() << "Here: " << QVariant(QSize(25, 60)).toString();

		Assert_Eq(settings.value(kWindowWidth), 800);
		Assert_Eq(settings.value(kWindowHeight), 600);
		Assert_Eq(settings.value(kWindowIsFullscreen), false);
	}
};

ADD_TESTCLASS(Test_Settings)
#include "Test_Settings.moc"
