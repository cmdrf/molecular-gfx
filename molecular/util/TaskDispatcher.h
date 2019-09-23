/*	TaskDispatcher.h

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

/** @file TaskDispatcher.h
	Selects TaskDispatcher for the current OS.
	@todo Implement StandardAsyncFileLoader
*/

#ifndef MOLECULAR_TASKDISPATCHER_H
#define MOLECULAR_TASKDISPATCHER_H

#if __APPLE__
#include <molecular/util/GcdTaskDispatcher.h>
#include <molecular/util/GcdAsyncFileLoader.h>

namespace molecular
{
typedef GcdTaskDispatcher TaskDispatcher;
using FileLoader = GcdAsyncFileLoader;
}
#elif !defined(__MINGW32__) // No std::thread implementation on current MinGW
#include <molecular/util/StdTaskQueue.h>
#include <molecular/util/DummyFileLoader.h>

namespace molecular
{
using TaskDispatcher = StdTaskQueue;
using FileLoader = DummyFileLoader;
}

#else
#include "util/DummyTaskDispatcher.h"
#include "util/DummyFileLoader.h"
namespace molecular
{
typedef DummyTaskDispatcher TaskDispatcher;
using FileLoader = DummyFileLoader;
}
#endif

#endif // MOLECULAR_TASKDISPATCHER_H
