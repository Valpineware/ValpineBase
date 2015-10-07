//=============================================================================|
// Copyright (C) 2015 Valpineware
// This file is licensed under the MIT License.
//=============================================================================|

#include <ValpineBase/Test.h>
#include <ValpineBase/Settings.h>

#include <QtCore/QDateTime>
#include <QtCore/QTemporaryFile>
#include <QtTest/QSignalSpy>

#include "private/Test_Settings/SampleKeyEnum.h"

using namespace vbase;

class Test_Settings : public test::Class
{
	Q_OBJECT

	using Key = QString;

private slots:
	VTEST void simpleCheck()
	{
		Settings<SampleKeyClass> settings;
		QTemporaryFile tmpFile;
		Assert_True(settings.load(tmpFile.fileName()));

		settings.setValue(SampleKeyClass::GraphicsWindowWidth, 800);
		settings.setValue(SampleKeyClass::GraphicsWindowHeight, 600);
		settings.setValue(SampleKeyClass::GraphicsWindowIsFullscreen, false);

		Verify_Eq(settings.value(SampleKeyClass::GraphicsWindowWidth), 800);
		Verify_Eq(settings.value(SampleKeyClass::GraphicsWindowHeight), 600);
		Verify_Eq(settings.value(SampleKeyClass::GraphicsWindowIsFullscreen), false);
	}


	VTEST void changeSignal()
	{
		Settings<SampleKeyClass> settings;
		QTemporaryFile tmpFile;
		Assert_True(settings.load(tmpFile.fileName()));


		struct {
			bool happened = false;
			SampleKeyClass::KeyEnum key;
			QVariant newValue;
		} signalResults;

		QObject::connect(&settings, &SettingsBase::valueChanged,
						 [&signalResults](int key,
											const QVariant &newValue)
		{
			signalResults.happened = true;
			signalResults.key = static_cast<SampleKeyClass::KeyEnum>(key);
			signalResults.newValue = newValue;
		});

		const QString testColor = "0x40a030";
		settings.setValue(SampleKeyClass::GraphicsWindowBackgroundColor, testColor);

		Verify_True(signalResults.happened);
		Verify_Eq(signalResults.key, SampleKeyClass::GraphicsWindowBackgroundColor);
		Verify_Eq(signalResults.newValue, testColor);
		Verify_Eq(settings.value(SampleKeyClass::GraphicsWindowBackgroundColor).toString(), testColor);
	}


	VTEST void defaultValues()
	{
		Settings<SampleKeyClass> settings;
		QTemporaryFile tmpFile;
		Assert_True(settings.load(tmpFile.fileName()));

		Verify_Eq(settings.value(SampleKeyClass::GraphicsWindowWidth), 800);
	}
};

ADD_TESTCLASS(Test_Settings)
#include "Test_Settings.moc"
