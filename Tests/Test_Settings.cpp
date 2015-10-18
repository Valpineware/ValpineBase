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

		AssertTrue(tmpFile.open());
		AssertTrue(settings.load(tmpFile.fileName()));
	}


	VTEST void simpleCheck()
	{
		settings.setValue(Key::GraphicsWindowWidth, 800);
		settings.setValue(Key::GraphicsWindowHeight, 600);
		settings.setValue(Key::GraphicsWindowIsFullscreen, false);

		VerifyEq(settings.value(Key::GraphicsWindowWidth), 800);
		VerifyEq(settings.value(Key::GraphicsWindowHeight), 600);
		VerifyEq(settings.value(Key::GraphicsWindowIsFullscreen), false);
	}


	VTEST void changeSignal()
	{
		QList<SignalResult> signalResults;
		checkValueChangedSignal(settings, signalResults);

		const QString testColor = "0x40a030";
		settings.setValue(Key::GraphicsWindowBackgroundColor, testColor);

		AssertEq(signalResults.count(), 1);
		VerifyEq(signalResults.first().key, Key::GraphicsWindowBackgroundColor);
		VerifyEq(signalResults.first().newValue, testColor);
		VerifyEq(settings.value(Key::GraphicsWindowBackgroundColor).toString(), testColor);
	}


	VTEST void defaultValues()
	{
		VerifyEq(settings.value(Key::GraphicsWindowWidth), 1600);
		VerifyEq(settings.value(Key::GraphicsWindowHeight), 900);
	}


	VTEST void enqueValue()
	{
		settings.setValue(Key::GraphicsWindowBackgroundColor, "blue");
		settings.enqueueValue(Key::GraphicsWindowBackgroundColor, "red");
		VerifyEq(settings.value(Key::GraphicsWindowBackgroundColor), "blue");

		QList<SignalResult> signalResults;
		checkValueChangedSignal(settings, signalResults);

		settings.setQueuedValues();
		AssertEq(signalResults.count(), 1);
		VerifyEq(signalResults.front().key, Key::GraphicsWindowBackgroundColor);
		VerifyEq(signalResults.front().newValue, "red");
		VerifyEq(settings.value(Key::GraphicsWindowBackgroundColor), "red");
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

		AssertEq(signalResults.count(), 3);

		//note we don't care about the order the settings were applied with
		//applyQueuedValues
		//TODO document this aspect
		VerifyTrue(signalResults.contains({ Key::GraphicsWindowHeight,
											QVariant(480)
					}));

		VerifyTrue(signalResults.contains({ Key::GraphicsWindowWidth,
											QVariant(640)
	}));

	VerifyTrue(signalResults.contains({ Key::GraphicsWindowBackgroundColor,
										QVariant("orange")
									  }));
}


VTEST void updateTypes()
{
	VerifyEq(settings.keyUpdateType(Key::GraphicsWindowHeight),
			KUT::Pending);
	VerifyEq(settings.keyUpdateType(Key::GraphicsWindowWidth),
			KUT::Pending);
	VerifyEq(settings.keyUpdateType(Key::GraphicsWindowIsFullscreen),
			  KUT::Pending);
	VerifyEq(settings.keyUpdateType(Key::GraphicsWindowBackgroundColor),
			 KUT::Instant);
}


};

ADD_TESTCLASS(Test_Settings)
#include "Test_Settings.moc"
