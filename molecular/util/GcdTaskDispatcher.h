/*	GcdTaskDispatcher.h
	Copyright 2013-2016 Fabian Herb

	This file is part of Molecular Engine.
*/

#ifndef GCDTASKDISPATCHER_H
#define GCDTASKDISPATCHER_H

#include <dispatch/dispatch.h>
#include <molecular/util/Task.h>

namespace molecular
{

class GcdTaskDispatcher
{
	friend class GcdAsyncFileLoader;
public:
	/// Mutex for synchronisation between tasks
	class Mutex : NonCopyable
	{
	public:
		Mutex() : mSemaphore(dispatch_semaphore_create(1)) {}
		~Mutex() {dispatch_release(mSemaphore);}

		void Lock() {dispatch_semaphore_wait(mSemaphore, DISPATCH_TIME_FOREVER);}
		void Unlock() {dispatch_semaphore_signal(mSemaphore);}

	private:
		dispatch_semaphore_t mSemaphore;
	};

	class FinishFlag
	{
		friend class GcdTaskDispatcher;
		friend class GcdAsyncFileLoader;
	public:
		FinishFlag()
		{
			mGroup = dispatch_group_create();
		}

		~FinishFlag()
		{
			dispatch_release(mGroup);
		}

		/** @deprecated */
		void Decrement()
		{
			dispatch_group_leave(mGroup);
		}


	private:
		dispatch_group_t mGroup;
	};

	using Task = TaskT<FinishFlag>;
	using FunctionTask = FunctionTaskT<FinishFlag>;

	GcdTaskDispatcher()
	{
		mQueue = dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0);
	}

	/** @deprecated Use std::function interface instead. */
	void EnqueueTask(Task* task, FinishFlag& flag)
	{
		dispatch_group_enter(flag.mGroup);
		task->SetFinishFlag(&flag);
		dispatch_async_f(mQueue, task, DispatchFunction);
	}

	/** @deprecated Use std::function interface instead. */
	void EnqueueTask(Task* task)
	{
		dispatch_async_f(mQueue, task, DispatchFunction);
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

	void WaitUntilFinished(FinishFlag& flag)
	{
		dispatch_group_wait(flag.mGroup, DISPATCH_TIME_FOREVER);
	}

	bool IsFinished(FinishFlag& flag)
	{
		return (dispatch_group_wait(flag.mGroup, DISPATCH_TIME_NOW) == 0);
	}

private:
	static void DispatchFunction(void* context);

	dispatch_queue_t mQueue;
};

}
#endif // GCDTASKDISPATCHER_H
