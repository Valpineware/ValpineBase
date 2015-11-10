//=============================================================================|
// Copyright (C) 2015 Valpineware
// This file is licensed under the MIT License.
//=============================================================================|

#include <thread>
#include <chrono>
#include <iostream>
#include <memory>

#include <ValpineBase/Concurrent/Latch.h>
#include <ValpineBase/Test.h>

using namespace vbase;

class Test_Latch : public test::Class
{
	Q_OBJECT

private slots:
	VTEST_ISOLATED VTIME_1 void noExecutionBeforeRelease()
	{
		concurrent::Latch latch;
		bool didStart = false;
		bool didExecute = false;

		std::thread t1([&]
		{
			didStart = true;
			latch.wait();
			didExecute = true;
		});

		VerifyTryTrue(didStart, 10);
		VerifyFalse(didExecute);
		latch.unlock();
		VerifyTryTrue(didExecute, 10);

		t1.join();
	}


	VTEST_ISOLATED VTIME_1 void waitingAfterUnlockShouldNotHang()
	{
		concurrent::Latch latch;
		bool didStart = false;
		bool didExecute = false;
		latch.unlock();

		std::thread t1([&]
		{
			didStart = true;
			latch.wait();
			didExecute = true;
		});

		VerifyTryTrue(didStart, 10);
		VerifyTryTrue(didExecute, 10);

		t1.join();
	}
};

ADD_TESTCLASS(Test_Latch)
#include "Test_Latch.moc"
