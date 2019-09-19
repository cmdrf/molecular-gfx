/*	StdThread.h
	Copyright 2015-2016 Fabian Herb

	This file is part of Molecular Engine.
*/

#ifndef STDTHREAD_H
#define STDTHREAD_H

#include <molecular/Config.h>
#include <thread>
#include <mutex>

/// Locking mechanism for resources accessed by multiple threads
class StdMutex
{
public:
	/// Blocks the caller if this mutex is locked, otherwise locks the mutex
	inline void Lock() {mMutex.lock();}

	/// Unlocks the mutex
	inline void Unlock() {mMutex.unlock();}

	/** For specialisation of MutexScopeGuard. */
	std::mutex& Get() {return mMutex;}

private:
	std::mutex mMutex;
};

/// Wrapper around std::thread
class StdThread
{
public:
	using Mutex = StdMutex;

	virtual ~StdThread() {WaitForExit();}

	/// This is the workload function
	virtual void Work() = 0;

	/// Starts execution of the thread
	void Start()
	{
		mThread = std::thread([this]{Work();});
	}

	/// Blocks the calling thread until this one ends
	void WaitForExit()
	{
		if(mThread.joinable())
			mThread.join();
	}

	static void Sleep(float seconds)
	{
#if _MSC_VER
		// For buggy Microsoft implementation
		const unsigned long milliseconds = seconds * 0.001;
		std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
#else
		std::this_thread::sleep_for(std::chrono::duration<float>(seconds));
#endif
	}

private:
	std::thread mThread;
};

#endif // STDTHREAD_H
