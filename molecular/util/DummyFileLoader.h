/*	DummyFileLoader.h
	Copyright 2015-2016 Fabian Herb

	This file is part of Molecular Engine.
*/

#ifndef DUMMYFILELOADER_H
#define DUMMYFILELOADER_H

#include "BlobFunctionTask.h"
#include "FileStreamStorage.h"

/// FileLoader that loads a file synchronously
class DummyFileLoader
{
public:
	template<class TDummy>
	DummyFileLoader(TDummy&) {}

	DummyFileLoader() = default;

	/// Read entire file and pass contents to handler
	/** @param path Path to file
		@param handler Function to call when file is loaded.
		@param handlerQueue Queue to run handler in. */
	template<class TQueue>
	void ReadFile(const char* path, std::function<void (Blob&)> handler, TQueue& handlerQueue, typename TQueue::FinishFlag& finishFlag, size_t offset = 0, size_t length = 0);

	template<class TQueue>
	void ReadFile(const char* path, std::function<void (Blob&)> handler, TQueue& handlerQueue, size_t offset = 0, size_t length = 0);

private:
	Blob ReadFile(const char* path, size_t offset, size_t length);
};

template<class TQueue>
void DummyFileLoader::ReadFile(const char* path, std::function<void (Blob&)> handler, TQueue& handlerQueue, typename TQueue::FinishFlag& finishFlag, size_t offset, size_t length)
{
	Blob blob = ReadFile(path, offset, length);
	handlerQueue.EnqueueTask(new BlobFunctionTask<TQueue>(handler, blob), finishFlag);
}

template<class TQueue>
void DummyFileLoader::ReadFile(const char* path, std::function<void (Blob&)> handler, TQueue& handlerQueue, size_t offset, size_t length)
{
	Blob blob = ReadFile(path, offset, length);
	handlerQueue.EnqueueTask(new BlobFunctionTask<TQueue>(handler, blob));
}

inline Blob DummyFileLoader::ReadFile(const char* path, size_t offset, size_t length)
{
	FileReadStorage storage(path);
	if(length == 0)
		length = storage.GetSize() - offset;
	if(offset != 0)
		storage.Skip(offset);
	Blob blob(length);
	size_t read = storage.Read(blob.GetData(), length);
	if(read != length)
		throw std::runtime_error("Cannot read file.");
	return blob;
}

#endif // DUMMYFILELOADER_H

