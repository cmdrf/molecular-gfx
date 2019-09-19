/*	StdTaskQueue.h
	Copyright 2016 Fabian Herb

	This file is part of Molecular Engine.
*/

#ifndef STDTASKQUEUE_H
#define STDTASKQUEUE_H

#include "AtomicCounter.h"
#include "Task.h"
#include <thread>
#include <mutex>
#include <condition_variable>
#include <deque>
#include <vector>
#include "NonCopyable.h"
#include "StdThread.h"

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

#endif
