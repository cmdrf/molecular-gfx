/*	SyncFileLoad.h
	Copyright 2016 Fabian Herb

	This file is part of Molecular Engine.
*/

#ifndef SYNCFILELOAD_H
#define SYNCFILELOAD_H

#include "Blob.h"
#include <memory>

namespace molecular
{

template<class TFileLoader, class TTaskQueue>
Blob SyncFileLoad(const char* filename, TFileLoader& fileLoader, TTaskQueue& queue, size_t offset = 0, size_t length = 0)
{
	Blob fileContents;
	auto handler = [&](Blob& blob){fileContents = std::move(blob);};
	typename TTaskQueue::FinishFlag flag;
	fileLoader.ReadFile(filename, handler, queue, flag, offset, length);
	queue.WaitUntilFinished(flag);
	return fileContents;
}

}
#endif // SYNCFILELOAD_H
