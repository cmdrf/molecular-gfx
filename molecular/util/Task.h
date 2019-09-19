/*	Task.h
	Copyright 2015-2016 Fabian Herb

	This file is part of Molecular Engine.
*/

#ifndef TASK_H
#define TASK_H

#include "NonCopyable.h"
#include <functional>

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


#endif // TASK_H

