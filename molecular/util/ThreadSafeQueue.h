/*	ThreadSafeQueue.h

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

#ifndef MOLECULAR_THREADSAFEQUEUE_H
#define MOLECULAR_THREADSAFEQUEUE_H

#include <queue>

namespace molecular
{
namespace util
{

/// Thread-safe FIFO queue
template<class T, class TMutex>
class ThreadSafeQueue
{
public:
	void Push(T& input)
	{
		mMutex.Lock();
		mQueue.push(input);
		mMutex.Unlock();
	}

	/// Pops an element from the queue
	/** @returns true if there was an element in the queue. False if the queue
			is empty, in which case output stays unchanged. */
	bool Pop(T& output)
	{
		mMutex.Lock();
		bool full = !mQueue.empty();
		if(full)
		{
			output = mQueue.front();
			mQueue.pop();
		}
		mMutex.Unlock();
		return full;
	}

private:
	std::queue<T> mQueue;
	TMutex mMutex;
};

}
}

#endif // MOLECULAR_THREADSAFEQUEUE_H
