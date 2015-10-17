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

private:
	//TODO extract to a macro with Test.h
	void tryVerifyTrue(bool &what, int timeLimitMs)
	{
		const int intervalMs = 5;
		const int tries = timeLimitMs / intervalMs;

		for (int i=0; i<tries; i++)
		{
			if (what)
				break;

			std::this_thread::sleep_for(std::chrono::milliseconds(intervalMs));
		}

		Verify_True(what);
	}


	void finalizeThread(std::thread &thread, bool didExecute)
	{
		//TODO what else can I do here?
		//there is no way to terminate the thread...
		if (!didExecute)
			thread.detach();
		else
			thread.join();
	}

private slots:
	VTEST void noExecutionBeforeRelease()
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

		tryVerifyTrue(didStart, 10);
		Verify_False(didExecute);
		latch.unlock();
		tryVerifyTrue(didExecute, 10);

		finalizeThread(t1, didExecute);
	}


	VTEST void waitingAfterUnlockShouldNotHang()
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

		tryVerifyTrue(didStart, 10);
		tryVerifyTrue(didExecute, 10);
		finalizeThread(t1, didExecute);
	}
};

ADD_TESTCLASS(Test_Latch)
#include "Test_Latch.moc"
