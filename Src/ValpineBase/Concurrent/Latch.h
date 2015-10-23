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

/**
 * The Latch class is a concurrency syncronization primitive that allows
 * multiple threads to wait until the Latch is unlocked from another thread.
 * Upon destruction, the Latch will unlock all waiting threads. The latch is
 * implemented with std::condition_variable, so waiting does not exhaust the CPU
 * as long as the std::condition_variable implementation is correct.
 *
 * Latch is thread-safe.
 *
 * Once a Latch is destroyed, there should be no threads attempting to access it.
 * Any threads currently blocking on wait() will have undefined behavior if the
 * Latch is destroyed.
 */
class Latch
{
public:
	/**
	 * Constructs a Latch in a locked state.
	 */
	Latch() :
		_mutexLock(new std::lock_guard<std::mutex>(_mutex))
	{
	}

	/**
	 * Blocks execution until the Latch is unlocked from another thread
	 * or is destructed.
	 */
	void wait() noexcept
	{
		std::unique_lock<std::mutex> lk(_mutex);
		_conditionVariable.wait(lk, []{ return true; });
	}


	/**
	 * Unlocks the Latch. All threads currently blocked on wait() are unblocked.
	 * TODO what about threads that haven't gotten to wait() yet?
	 */
	void unlock() noexcept
	{
		_mutexLock = nullptr;
		_conditionVariable.notify_all();
	}

private:
	std::condition_variable _conditionVariable;
	std::mutex _mutex;
	std::unique_ptr<std::lock_guard<std::mutex>> _mutexLock = nullptr;
};

END_NAMESPACE
END_NAMESPACE

#endif
