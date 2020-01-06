/*	ManualTaskQueue.h

MIT License

Copyright (c) 2019-2020 Fabian Herb

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

#ifndef MOLECULAR_MANUALTASKQUEUE_H
#define MOLECULAR_MANUALTASKQUEUE_H

#include "ThreadSafeQueue.h"
#include "Task.h"
#include "AtomicCounter.h"
#include "NonCopyable.h"

namespace molecular
{
namespace util
{

/// Task queue that runs tasks on calls to RunOneTask
/** @see DummyTaskDispatcher */
template<class TMutex>
class ManualTaskQueue : NonCopyable
{
public:
	typedef AtomicCounter FinishFlag;
	using Task = TaskT<FinishFlag>;
	using FunctionTask = FunctionTaskT<FinishFlag>;
	using Mutex = TMutex;

	~ManualTaskQueue();

	void RunOneTask();
	bool Empty() {return mQueue.empty();}

	/** @deprecated Use std::function interface instead. */
	void EnqueueTask(Task* task) {mQueue.Push(task);}

	/** @deprecated Use std::function interface instead. */
	void EnqueueTask(Task* /*task*/, FinishFlag& /*flag*/)
	{
		assert(false);
		//flag.Increment();
		//task->SetFinishFlag(&flag);
		//mQueue.Push(task);
	}

	/// Asynchronously execute function
	void EnqueueTask(std::function<void ()>&& function, FinishFlag& flag)
	{
		EnqueueTask(new FunctionTask(std::move(function)), flag);
	}

	/// Asynchronously execute function
	void EnqueueTask(std::function<void ()>&& function)
	{
		EnqueueTask(new FunctionTask(std::move(function)));
	}

	bool IsFinished(FinishFlag& flag);

private:
	ThreadSafeQueue<Task*, Mutex> mQueue;
};

/*****************************************************************************/

template<class TMutex>
ManualTaskQueue<TMutex>::~ManualTaskQueue()
{
	Task* task = nullptr;
	while(mQueue.Pop(task))
		delete task;
}

template<class TMutex>
void ManualTaskQueue<TMutex>::RunOneTask()
{
	Task* task = nullptr;
	if(mQueue.Pop(task))
	{
		assert(task);
		task->Run();
		delete task;
	}
}


template<class TMutex>
bool ManualTaskQueue<TMutex>::IsFinished(FinishFlag& flag)
{
	return flag.CheckZero();
}

}
}

#endif // MOLECULAR_MANUALTASKQUEUE_H

