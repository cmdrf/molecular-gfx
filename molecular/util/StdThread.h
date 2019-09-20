/*	StdThread.h

MIT License

Copyright (c) 2019 Fabian Herb

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#ifndef MOLECULAR_STDTHREAD_H
#define MOLECULAR_STDTHREAD_H

#include <molecular/Config.h>
#include <thread>
#include <mutex>

namespace molecular
{

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

}

#endif // MOLECULAR_STDTHREAD_H
