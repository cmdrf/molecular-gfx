/*	StdTaskQueue.cpp

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
SOFTWARE.*/

#include "StdTaskQueue.h"
#include <cassert>
#include <algorithm>

namespace molecular
{

StdTaskQueue::StdTaskQueue() :
	mStop(false)
{
	unsigned int numThreads = std::max(std::thread::hardware_concurrency(), 2u);
	// Start worker threads
	for(unsigned int i = 0; i < numThreads; ++i)
		mWorkers.push_back(std::thread([&]() {Work();}));
}

StdTaskQueue::~StdTaskQueue()
{
	{
		std::unique_lock<std::mutex> lock(mQueueMutex);
		mStop = true;
	}
	mCondition.notify_all();

	for(auto& worker: mWorkers)
		worker.join();
}

void StdTaskQueue::EnqueueTask(Task* task)
{
	assert(task);
	{
		std::unique_lock<std::mutex> lock(mQueueMutex);
		mQueue.push_back(task);
	}
	// wake up one thread
	mCondition.notify_one();
}

void StdTaskQueue::EnqueueTask(Task* task, FinishFlag& flag)
{
	task->SetFinishFlag(&flag);
	flag.Increment();
	EnqueueTask(task);
}

void StdTaskQueue::WaitUntilFinished(FinishFlag& flag)
{
	WorkLoop([&](){return mStop || flag.CheckZero();});
}

bool StdTaskQueue::IsFinished(FinishFlag& flag)
{
	return flag.CheckZero();
}

void StdTaskQueue::Work()
{
	WorkLoop([&](){return mStop;});
}

template<class TCancelPredicate>
void StdTaskQueue::WorkLoop(TCancelPredicate cancelPredicate)
{
	Task* task = nullptr;
	while(true)
	{
		{
			std::unique_lock<std::mutex> lock(mQueueMutex);

			while(!cancelPredicate() && mQueue.empty())
			{
				// Wait on condition variable
				mCondition.wait(lock);
			}

			if(cancelPredicate())
				return;

			// get the task from the queue
			task = mQueue.front();
			mQueue.pop_front();
		}

		// Execute the task:
		assert(task);
		task->Run();
		FinishFlag* flag = task->GetFinishFlag();
		delete task;

		// If there is a FinishFlag and it is zero, wake all threads
		if(flag && flag->CheckZero())
		{
			std::unique_lock<std::mutex> lock(mQueueMutex);
			// TODO: Decrease flag here?
			mCondition.notify_all(); // TODO: Notify after unlocking?
		}
	}
}

}
