/*	DummyFileLoader.h

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

#ifndef MOLECULAR_DUMMYFILELOADER_H
#define MOLECULAR_DUMMYFILELOADER_H

#include "BlobFunctionTask.h"
#include <molecular/util/FileStreamStorage.h>

namespace molecular
{

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

}

#endif // MOLECULAR_DUMMYFILELOADER_H

