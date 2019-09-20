/*	ManualTaskQueue.h
	Copyright 2015-2016 Fabian Herb

	This file is part of Molecular Engine.
*/

#ifndef MOLECULAR_MANUALTASKQUEUE_H
#define MOLECULAR_MANUALTASKQUEUE_H

#include "ThreadSafeQueue.h"
#include "Task.h"
#include "AtomicCounter.h"
#include "NonCopyable.h"

namespace molecular
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
	void EnqueueTask(Task* task, FinishFlag& flag)
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

#endif // MOLECULAR_MANUALTASKQUEUE_H

