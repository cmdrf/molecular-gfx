/*	BlobFunctionTask.h
	Copyright 2015-2016 Fabian Herb

	This file is part of Molecular Engine.
*/

#ifndef BLOBFUNCTIONTASK_H
#define BLOBFUNCTIONTASK_H

#include "Task.h"
#include "Blob.h"
#include <functional>

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

#endif // BLOBFUNCTIONTASK_H

