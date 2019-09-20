/*	GcdTaskDispatcher.cpp
	Copyright 2013-2016 Fabian Herb

	This file is part of Molecular Engine.
*/

#include "util/GcdTaskDispatcher.h"
#include "util/TaskDispatcher.h"

void GcdTaskDispatcher::DispatchFunction(void* context)
{
	Task* task = static_cast<Task*>(context);
	task->Run();
	delete task;
}
