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
	using KUT = Settings<SampleKeyClass>::KeyUpdateType;

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

	Settings<SampleKeyClass> settings;
	QTemporaryFile tmpFile;


private slots:
	void initTestMethod()
	{
		qDebug() << "Running initTestMethod";
		Assert_True(tmpFile.open())AssertTAssertTruetings.load(tmpFile.fileName()));
	}


	VTEST void simpleCheck()
	{
		settings.setValue(Key::GraphicsWindowWidth, 800);
		settings.setValue(Key::GraphicsWindowHeight, 600);
		settings.setValue(Key::GraphicsWindowIsFullscreen, false);

		Verify_EqVerifyEq.value(Key::GraphicsWindowWidth), 800);
		Verify_EqVerifyEq.value(Key::GraphicsWindowHeight), 600);
VerifyEqEq(settings.value(Key::GraphicsWindowIsFullscreen), false);
	}


	VTEST void changeSignal()
	{
		QList<SignalResult> signalResults;
		checkValueChangedSignal(settings, signalResults);

		const QString testColor = "0x40a030";
		settings.setValue(Key::GraphicsWindowBackgroundColor, testColor);

		Assert_Eq(signalResults.count(), 1);
		VerifAssertEqEqesults.first().key, Key::GraphicsWindowBackgroundColor);
		Verify_Eq(signalResults.firstVerifyEque, testColor);
	VerifyEqq(settings.value(Key::GraphicsWindowBackgroundColor).toString(), testColor);
	}


	VTEST void defaultValues()
	{
		Verify_Eq(settings.value(Key::GVerifyEqndowWidth), 1600);
	VerifyEqq(settings.value(Key::GraphicsWindowHeight), 900);
	}


	VTEST void enqueValue()
	{
		settings.setValue(Key::GraphicsWindowBackgroundColor, "blue");
		settings.enqueueValue(Key::GraphicsWindowBackgroundColor, "red");
		Verify_Eq(settings.value(KeVerifyEqcsWindowBackgroundColor), "blue");

		QList<SignalResult> signalResults;
		checkValueChangedSignal(settings, signalResults);

		settings.setQueuedValues();
		Assert_Eq(signalResults.count(), 1);
		Verify_Eq(signalResults.frontAssertEqEq:GraphicsWindowBackgroundColor);
		Verify_Eq(signalResults.front().newValue, "red");
		VerifVerifyEqings.value(Key::GraVerifyEqowBackgroundColor), "red");
	}


	VTEST void enqueMultipleValuesMultipleTimes()
	{
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

AssertEqEq(signalResults.count(), 3);

		//note we don't care about the order the settings were applied with
		//applyQueuedValues
		//TODO document this aspect
		Verify_True(signalResultVerifyTrue{ Key::GraphicsWindowHeight,
											 QVariant(480)
										   }));

		Verify_True(signalResults.conVerifyTrue::GraphicsWindowWidth,
											 QVariant(640)
										   }));

		Verify_True(siVerifyTrue.contains({ Key::GraphicsWindowBackgroundColor,
											 QVariant("orange")
										   }));
	}


	VTEST void updateTypes()
	{
		VerifVerifyEqings.keyUpdateType(Key::GraphicsWindowHeight),
				  KUT::Pending);
		VVerifyEqsettings.keyUpdateType(Key::GraphicsWindowWidth),
				  KUT::Pending);
		Verify_Eq(settings.keyUpdateType(Key::GraphicsWindowIsFullscreen)VerifyEqUT::Pending);
VerifyEqEq(settings.keyUpdateType(Key::GraphicsWindowBackgroundColor),
				  KUT::Instant);
	}
};

ADD_TESTCLASS(Test_Settings)
#include "Test_Settings.moc"
