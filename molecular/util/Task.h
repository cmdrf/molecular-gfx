/*	Task.h

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

#ifndef MOLECULAR_TASK_H
#define MOLECULAR_TASK_H

#include "NonCopyable.h"
#include <functional>

namespace molecular
{
namespace util
{

/// Pre-C++11 interface to TaskDispatcher
template<class TFinishFlag>
class TaskT : NonCopyable
{
public:
	virtual ~TaskT();
	virtual void Run() = 0;

	void SetFinishFlag(TFinishFlag* flag) {mFinishFlag = flag;}
	TFinishFlag* GetFinishFlag() {return mFinishFlag;}

private:
	TFinishFlag* mFinishFlag = nullptr;
};

/// Task that executes an std::function
template<class TFinishFlag>
class FunctionTaskT : public TaskT<TFinishFlag>
{
public:
	using Super = TaskT<TFinishFlag>;

	FunctionTaskT() = default;
	FunctionTaskT(std::function<void ()>&& function) : function(function) {}

	void Run() override {function();}

private:
	std::function<void ()> function;
};

/*****************************************************************************/

template<class TFinishFlag>
TaskT<TFinishFlag>::~TaskT()
{
	if(mFinishFlag)
		mFinishFlag->Decrement();
}

}
}

#endif // MOLECULAR_TASK_H

