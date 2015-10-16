//=============================================================================|
// Copyright (C) 2015 Valpineware
// This file is licensed under the MIT License.
//=============================================================================|

#ifndef _ValpineBase_Concurrent_Latch_h
#define _ValpineBase_Concurrent_Latch_h

#include <mutex>
#include <condition_variable>

#include <ValpineBase/ValpineBase.h>

namespace vbase { namespace concurrent {

class Latch
{
public:
	Latch() :
		_mutexLock(new std::lock_guard<std::mutex>(_mutex))
	{
	}


	void wait() noexcept
	{
		std::unique_lock<std::mutex> lk(_mutex);
		_conditionVariable.wait(lk, []{ return true; });
	}


	void unlock() noexcept
	{
		delete _mutexLock;
		_mutexLock = nullptr;
		_conditionVariable.notify_all();
	}

private:
	std::condition_variable _conditionVariable;
	std::mutex _mutex;
	std::lock_guard<std::mutex> *_mutexLock = nullptr;
};

END_NAMESPACE
END_NAMESPACE


#endif
