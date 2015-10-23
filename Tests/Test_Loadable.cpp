//=============================================================================|
// Copyright (C) 2015 Valpineware
// This file is licensed under the MIT License.
//=============================================================================|

#include <thread>
#include <chrono>

#include <QtCore/QDateTime>

#include <ValpineBase/Test.h>
#include <ValpineBase/Loadable.h>

using namespace vbase;

class Test_Loadable : public test::Class
{
	Q_OBJECT

private slots:
	VTEST void simpleCheck()
	{
		Loadable loadable;

		bool didIsLoading = false;
		loadable.pIsLoading.addOnChangedListener([&didIsLoading]
		{
			didIsLoading = true;
		});

		auto f = [&]
		{
			Loadable::Begin b(&loadable);
		};


		AssertFalse(loadable.pHasLoaded);
		f();
		AssertTrue(loadable.pHasLoaded);
		AssertTrue(didIsLoading);

	}


	VTEST_ISOLATED void runMeAloneCauseIWontFinish()
	{
		std::thread([]
		{
			std::this_thread::sleep_for(std::chrono::hours(15));
		});
	}
};

ADD_TESTCLASS(Test_Loadable)
#include "Test_Loadable.moc"
