/*	BlobFunctionTask.h

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

#ifndef MOLECULAR_BLOBFUNCTIONTASK_H
#define MOLECULAR_BLOBFUNCTIONTASK_H

#include "Task.h"
#include "Blob.h"
#include <functional>

namespace molecular
{

/// Task that calls an std::function with a Blob argument
/** Blob is movable only, std::function is copyable, so std::bind cannot be
	used.
	@todo Consider using std::packaged_task instead, which is movable-only
	@see GcdAsyncFileLoader
	@see DummyFileLoader
	@see StandardFileLoader */
template <class TQueue>
class BlobFunctionTask : public TQueue::Task
{
public:

	BlobFunctionTask(std::function<void (Blob&)>& function, Blob& blob) :
		mFunction(function),
		mBlob(std::move(blob))
	{}

	void Run() override {mFunction(mBlob);}

private:
	std::function<void (Blob&)> mFunction;
	Blob mBlob;
};

}

#endif // MOLECULAR_BLOBFUNCTIONTASK_H

