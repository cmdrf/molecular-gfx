/*	StdTaskQueue.h

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

#ifndef MOLECULAR_STDTASKQUEUE_H
#define MOLECULAR_STDTASKQUEUE_H

#include "AtomicCounter.h"
#include "Task.h"
#include <thread>
#include <mutex>
#include <condition_variable>
#include <deque>
#include <vector>
#include "NonCopyable.h"
#include "StdThread.h"

namespace molecular
{

class StdTaskQueue : NonCopyable
{
public:
	typedef AtomicCounter FinishFlag;
	using Task = TaskT<FinishFlag>;
	using FunctionTask = FunctionTaskT<FinishFlag>;
	using Mutex = StdMutex;

	StdTaskQueue();
	~StdTaskQueue();

	/** @deprecated Use std::function interface instead. */
	void EnqueueTask(Task* task);

	void EnqueueTask(Task* task, FinishFlag& flag);

	/// Asynchronously execute function
	void EnqueueTask(std::function<void()>&& function, FinishFlag& flag)
	{
		EnqueueTask(new FunctionTask(std::move(function)), flag);
	}

	/// Asynchronously execute function
	void EnqueueTask(std::function<void()>&& function)
	{
		EnqueueTask(new FunctionTask(std::move(function)));
	}

	void WaitUntilFinished(FinishFlag& flag);
	bool IsFinished(FinishFlag& flag);

private:
	void Work();

	template<class TCancelPredicate>
	void WorkLoop(TCancelPredicate cancelPredicate);

	std::vector<std::thread> mWorkers;
	std::deque<Task*> mQueue;
	std::mutex mQueueMutex;
	std::condition_variable mCondition;
	bool mStop;
};

}

#endif
