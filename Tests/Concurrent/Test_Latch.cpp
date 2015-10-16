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

private slots:
	VTEST void noExecutionBeforeRelease()
	{
		bool didStart = false;
		bool didExecute = false;
		concurrent::Latch latch;
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

		//TODO what else can I do here? there is no way to terminate the thread...
		if (!didExecute)
			t1.detach();
		else
			t1.join();
	}


	VTEST void raiiBehavior()
	{
		bool didStart = false;
		bool didExecute = false;
		std::unique_ptr<std::thread> t1;

		{
			concurrent::Latch latch;

			t1 = std::unique_ptr<std::thread>(new std::thread([&]
			{
				didStart = true;
				latch.wait();
				didExecute = true;
			}));

			tryVerifyTrue(didStart, 10);
			Verify_False(didExecute);
		}

		tryVerifyTrue(didExecute, 10);

		//TODO what else can I do here? there is no way to terminate the thread...
		if (!didExecute)
			t1->detach();
		else
			t1->join();
	}
};

ADD_TESTCLASS(Test_Latch)
#include "Test_Latch.moc"
