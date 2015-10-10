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


private:
	struct SignalResult
	{
		SampleKeyClass::KeyEnum key;
		QVariant newValue;

		bool operator ==(const SignalResult &signalResult) const
		{
			return (key == signalResult.key) &&
					(newValue == signalResult.newValue);
		}
	};


	void checkValueChangedSignal(Settings<SampleKeyClass> &settings,
								 QList<SignalResult> &signalResultList)
	{
		QObject::connect(&settings, &SettingsBase::valueChanged,
						 [&signalResultList](int key,
											const QVariant &newValue)
		{
			SignalResult sr;
			sr.key = static_cast<SampleKeyClass::KeyEnum>(key);
			sr.newValue = newValue;
			signalResultList.append(sr);
		});
	}


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

		QList<SignalResult> signalResults;
		checkValueChangedSignal(settings, signalResults);

		const QString testColor = "0x40a030";
		settings.setValue(Key::GraphicsWindowBackgroundColor, testColor);

		Assert_Eq(signalResults.count(), 1);
		Verify_Eq(signalResults.first().key, Key::GraphicsWindowBackgroundColor);
		Verify_Eq(signalResults.first().newValue, testColor);
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

		QList<SignalResult> signalResults;
		checkValueChangedSignal(settings, signalResults);

		settings.setQueuedValues();
		Assert_Eq(signalResults.count(), 1);
		Verify_Eq(signalResults.front().key, Key::GraphicsWindowBackgroundColor);
		Verify_Eq(signalResults.front().newValue, "red");
		Verify_Eq(settings.value(Key::GraphicsWindowBackgroundColor), "red");
	}


	VTEST void enqueMultipleValues()
	{

	}


	VTEST void enqueMultipleValuesMultipleTimes()
	{
		Settings<SampleKeyClass> settings;
		QTemporaryFile tmpFile;
		Assert_True(tmpFile.open());
		Assert_True(settings.load(tmpFile.fileName()));

		QList<SignalResult> signalResults;
		checkValueChangedSignal(settings, signalResults);

		//do stuff to settings
		{
			settings.enqueueValue(Key::GraphicsWindowHeight, 480);
			settings.enqueueValue(Key::GraphicsWindowWidth, 640);
			settings.setQueuedValues();

			settings.enqueueValue(Key::GraphicsWindowBackgroundColor, "green");
			settings.enqueueValue(Key::GraphicsWindowBackgroundColor, "orange");
			settings.setQueuedValues();
		}

		Assert_Eq(signalResults.count(), 3);

		//note we don't care about the order the settings were applied with
		//applyQueuedValues
		//TODO document this aspect
		Verify_True(signalResults.contains({ Key::GraphicsWindowHeight,
											 QVariant(480)
										   }));

		Verify_True(signalResults.contains({ Key::GraphicsWindowWidth,
											 QVariant(640)
										   }));

		Verify_True(signalResults.contains({ Key::GraphicsWindowBackgroundColor,
											 QVariant("orange")
										   }));
	}


	VTEST void enqueValuesBeforeAndAfterApply()
	{

	}
};

ADD_TESTCLASS(Test_Settings)
#include "Test_Settings.moc"
