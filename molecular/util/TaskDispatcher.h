/*	TaskDispatcher.h
	Copyright 2012-2016 Fabian Herb

	This file is part of Molecular Engine.
*/

/** @file TaskDispatcher.h
	Selects TaskDispatcher for the current OS.
	@todo Implement StandardAsyncFileLoader
*/

#ifndef TASKDISPATCHER_H
#define TASKDISPATCHER_H

#if __APPLE__
#include "util/GcdTaskDispatcher.h"
#include "util/GcdAsyncFileLoader.h"
typedef GcdTaskDispatcher TaskDispatcher;
using FileLoader = GcdAsyncFileLoader;
#elif !defined(__MINGW32__) // No std::thread implementation on current MinGW
#include <molecular/util/StdTaskQueue.h>
#include <molecular/util/DummyFileLoader.h>

using TaskDispatcher = StdTaskQueue;
using FileLoader = DummyFileLoader;

#else
#include "util/DummyTaskDispatcher.h"
#include "util/DummyFileLoader.h"
typedef DummyTaskDispatcher TaskDispatcher;
using FileLoader = DummyFileLoader;
#endif

#endif // TASKDISPATCHER_H
