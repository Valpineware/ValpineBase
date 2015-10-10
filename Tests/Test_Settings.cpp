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

	using Key = SampleKeyClass::KeyEnum;

private slots:
	VTEST void simpleCheck()
	{
		//FIXME add initTestCase and cleanupTestCase to test::Class so we can
		//create thise settings instance for all methods
		Settings<SampleKeyClass> settings;
		QTemporaryFile tmpFile;
		Assert_True(tmpFile.open());
		Assert_True(settings.load(tmpFile.fileName()));

		settings.setValue(Key::GraphicsWindowWidth, 800);
		settings.setValue(Key::GraphicsWindowHeight, 600);
		settings.setValue(Key::GraphicsWindowIsFullscreen, false);

		Verify_Eq(settings.value(Key::GraphicsWindowWidth), 800);
		Verify_Eq(settings.value(Key::GraphicsWindowHeight), 600);
		Verify_Eq(settings.value(Key::GraphicsWindowIsFullscreen), false);
	}


	VTEST void changeSignal()
	{
		Settings<SampleKeyClass> settings;
		QTemporaryFile tmpFile;
		Assert_True(tmpFile.open());
		Assert_True(settings.load(tmpFile.fileName()));

		struct
		{
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
		settings.setValue(Key::GraphicsWindowBackgroundColor, testColor);

		Verify_True(signalResults.happened);
		Verify_Eq(signalResults.key, Key::GraphicsWindowBackgroundColor);
		Verify_Eq(signalResults.newValue, testColor);
		Verify_Eq(settings.value(Key::GraphicsWindowBackgroundColor).toString(), testColor);
	}


	VTEST void defaultValues()
	{
		Settings<SampleKeyClass> settings;
		QTemporaryFile tmpFile;
		Assert_True(tmpFile.open());
		Assert_True(settings.load(tmpFile.fileName()));

		Verify_Eq(settings.value(Key::GraphicsWindowWidth), 1600);
		Verify_Eq(settings.value(Key::GraphicsWindowHeight), 900);
	}

	VTEST void enqueValue()
	{
		Settings<SampleKeyClass> settings;
		QTemporaryFile tmpFile;
		Assert_True(tmpFile.open());
		Assert_True(settings.load(tmpFile.fileName()));

		settings.setValue(Key::GraphicsWindowBackgroundColor, "blue");
		settings.enqueueValue(Key::GraphicsWindowBackgroundColor, "red");
		Verify_Eq(settings.value(Key::GraphicsWindowBackgroundColor), "blue");

		struct SignalResult
		{
			SampleKeyClass::KeyEnum key;
			QVariant newValue;
		};

		QList<SignalResult> signalResults;

		QObject::connect(&settings, &SettingsBase::valueChanged,
						 [&signalResults](int key,
											const QVariant &newValue)
		{
			SignalResult sr;
			sr.key = static_cast<SampleKeyClass::KeyEnum>(key);
			sr.newValue = newValue;
			signalResults.append(sr);
		});

		settings.applyQueuedValues();
		Assert_Eq(signalResults.count(), 1);
		Verify_Eq(signalResults.front().key, Key::GraphicsWindowBackgroundColor);
		Verify_Eq(signalResults.front().newValue, "red");
		Verify_Eq(settings.value(Key::GraphicsWindowBackgroundColor), "red");
	}


	VTEST void enqueMultipleValues()
	{

	}


	VTEST void enqueValueTwice()
	{
		Settings<SampleKeyClass> settings;
		QTemporaryFile tmpFile;
		Assert_True(tmpFile.open());
		Assert_True(settings.load(tmpFile.fileName()));

		settings.setValue(Key::GraphicsWindowBackgroundColor, "blue");
		settings.enqueueValue(Key::GraphicsWindowBackgroundColor, "red");
		Verify_Eq(settings.value(Key::GraphicsWindowBackgroundColor), "blue");

		settings.applyQueuedValues();
		Verify_Eq(settings.value(Key::GraphicsWindowBackgroundColor), "red");
	}


	VTEST void enqueValuesBeforeAndAfterApply()
	{

	}
};

ADD_TESTCLASS(Test_Settings)
#include "Test_Settings.moc"
