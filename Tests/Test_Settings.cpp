//=============================================================================|
// Copyright (C) 2015 Valpineware
// This file is licensed under the MIT License.
//=============================================================================|

#include <ValpineBase/Test.h>
#include <ValpineBase/Settings.h>

#include <QtCore/QDateTime>
#include <QtCore/QTemporaryFile>
#include <QtTest/QSignalSpy>

using namespace vbase;

class Test_Settings : public test::Class
{
	Q_OBJECT

	using Key = QString;

private slots:
	VTEST void simpleCheck()
	{
		const Key kWindowWidth = "Window/Width";
		const Key kWindowHeight = "Window/Height";
		const Key kWindowIsFullscreen = "Window/IsFullScreen";

		Settings settings;
		//TODO get a better way to generate an isolated directory for files
		Assert_True(settings.load("MyHackFileFixLater.ini"));

		settings.setValue(kWindowWidth, 800);
		settings.setValue(kWindowHeight, 600);
		settings.setValue(kWindowIsFullscreen, false);

		Assert_Eq(settings.value(kWindowWidth), 800);
		Assert_Eq(settings.value(kWindowHeight), 600);
		Assert_Eq(settings.value(kWindowIsFullscreen), false);
	}


	VTEST void changeSignal()
	{
		const Key kBackgroundColor = "BackgroundColor";

		Settings settings;
		//TODO
		Assert_True(settings.load("MyHack2.ini"));

		struct {
			bool happened = false;
			QString key;
			QVariant newValue;
		} signalResults;

		QObject::connect(&settings, &Settings::valueChanged,
						 [&signalResults](const QString &key, const QVariant &newValue)
		{
			signalResults.happened = true;
			signalResults.key = key;
			signalResults.newValue = newValue;
		});

		const QString testColor = "0x40a030";
		settings.setValue(kBackgroundColor, testColor);

		Assert_True(signalResults.happened);
		Assert_Eq(signalResults.key, kBackgroundColor);
		Assert_Eq(signalResults.newValue, testColor);
		Assert_Eq(settings.value(kBackgroundColor).toString(), testColor);
	}
};

ADD_TESTCLASS(Test_Settings)
#include "Test_Settings.moc"
