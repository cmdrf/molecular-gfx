/*	GcdAsyncFileLoader.h

MIT License

Copyright (c) 2019-2020 Fabian Herb

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

#ifndef MOLECULAR_GCDASYNCFILELOADER_H
#define MOLECULAR_GCDASYNCFILELOADER_H

#include <molecular/util/BlobFunctionTask.h>
#include <functional>
#include <molecular/util/GcdTaskDispatcher.h>
#include <stdexcept>
#include <sys/stat.h>
#include <iostream>

namespace molecular
{
namespace util
{

/// Loads files asynchronously using Grand Central Dispatch
class GcdAsyncFileLoader
{
public:
	GcdAsyncFileLoader(GcdTaskDispatcher& queue) : mQueue(queue) {}

	/// Asynchronously read entire file and pass contents to handler
	/** @param path Path to file
		@param handler Function to call when file is loaded.
		@param handlerQueue Queue to run handler in. */
	template<class TQueue>
	void ReadFile(const char* path, std::function<void (Blob&)> handler, TQueue& handlerQueue, size_t offset = 0, size_t length = 0);

	/// ReadFile with FinishFlag
	/** Only legal for GcdTaskDispatcher. */
	void ReadFile(const char* path, std::function<void (Blob&)> handler, GcdTaskDispatcher& handlerQueue, GcdTaskDispatcher::FinishFlag& finishFlag, size_t offset = 0, size_t length = 0);

private:
	template <class TQueue>
	struct ReadContext;

	template<class TQueue>
	dispatch_source_t SetupSource(const char* path, std::function<void (Blob&)> handler, TQueue& handlerQueue, typename TQueue::FinishFlag* finishFlag, size_t offset, size_t length);

	template<class TQueue>
	static void ReadCancelHandler(void* context);

	static void ReadCancelHandlerWithFinishFlag(void* context);

	template<class TQueue>
	static void ReadEventHandler(void* context);

	GcdTaskDispatcher& mQueue;
};

/*****************************************************************************/

template <class TQueue>
struct GcdAsyncFileLoader::ReadContext
{
	Blob blob;
	std::function<void (Blob&)> handler;
	GcdTaskDispatcher& readQueue;
	TQueue& handlerQueue;
	typename TQueue::FinishFlag* finishFlag;
	int fileDescriptor;
	size_t completed;
	dispatch_source_t dispatchSource;
};

template<class TQueue>
void GcdAsyncFileLoader::ReadFile(const char* path, std::function<void (Blob&)> handler, TQueue& handlerQueue, size_t offset, size_t length)
{
	dispatch_source_t dispatchSource = SetupSource(path, handler, handlerQueue, nullptr, offset, length);
	dispatch_source_set_cancel_handler_f(dispatchSource, ReadCancelHandler<TQueue>);
	dispatch_resume(dispatchSource);
}

inline void GcdAsyncFileLoader::ReadFile(const char* path, std::function<void (Blob&)> handler, GcdTaskDispatcher& handlerQueue, GcdTaskDispatcher::FinishFlag& finishFlag, size_t offset, size_t length)
{
	dispatch_source_t dispatchSource = SetupSource(path, handler, handlerQueue, &finishFlag, offset, length);
	dispatch_source_set_cancel_handler_f(dispatchSource, ReadCancelHandlerWithFinishFlag);
	dispatch_group_enter(finishFlag.mGroup);
	dispatch_resume(dispatchSource);
}

template<class TQueue>
dispatch_source_t GcdAsyncFileLoader::SetupSource(const char* path, std::function<void (Blob&)> handler, TQueue& handlerQueue, typename TQueue::FinishFlag* finishFlag, size_t offset, size_t length)
{
	int fd = open(path, O_RDONLY | O_NONBLOCK);
	if(fd == -1)
		throw std::runtime_error("GcdAsyncFileLoader: Could not open file " + std::string(path));

	if(length == 0)
	{
		struct stat output;
		if(fstat(fd, &output) != 0)
			throw std::runtime_error("GcdAsyncFileLoader: fstat on " + std::string(path) + " failed");
		if(output.st_size == 0)
			throw std::runtime_error("GcdAsyncFileLoader: Empty file " + std::string(path));
		length = output.st_size - offset;
	}

	if(offset != 0)
		lseek(fd, offset, SEEK_SET);
	dispatch_source_t dispatchSource = dispatch_source_create(DISPATCH_SOURCE_TYPE_READ, fd, 0, mQueue.mQueue);

	if(!dispatchSource)
	{
		close(fd);
		throw std::runtime_error("GcdAsyncFileLoader: Could not create dispatch source");
	}

	ReadContext<TQueue>* c = new ReadContext<TQueue>
	{
		Blob(length),
		handler,
		mQueue,
		handlerQueue,
		finishFlag,
		fd,
		0,
		dispatchSource
	};
	dispatch_set_context(dispatchSource, c);
	dispatch_source_set_event_handler_f(dispatchSource, ReadEventHandler<TQueue>);

	return dispatchSource;
}

template<class TQueue>
void GcdAsyncFileLoader::ReadCancelHandler(void* context)
{
	ReadContext<TQueue>* c = static_cast<ReadContext<TQueue>*>(context);
	close(c->fileDescriptor);
	c->handlerQueue.EnqueueTask(new BlobFunctionTask<TQueue>(c->handler, c->blob));

	dispatch_release(c->dispatchSource);
	delete c;
}

inline void GcdAsyncFileLoader::ReadCancelHandlerWithFinishFlag(void* context)
{
	ReadContext<GcdTaskDispatcher>* c = static_cast<ReadContext<GcdTaskDispatcher>*>(context);
	close(c->fileDescriptor);
	c->handlerQueue.EnqueueTask(new BlobFunctionTask<GcdTaskDispatcher>(c->handler, c->blob), *c->finishFlag);
	dispatch_group_leave(c->finishFlag->mGroup);

	dispatch_release(c->dispatchSource);
	delete c;
}

template<class TQueue>
void GcdAsyncFileLoader::ReadEventHandler(void* context)
{
	ReadContext<TQueue>* readContext = static_cast<ReadContext<TQueue>*>(context);
	uint8_t* destination = readContext->blob.GetBytes() + readContext->completed;
	size_t bytesToRead = readContext->blob.GetSize() - readContext->completed;
	ssize_t result = read(readContext->fileDescriptor, destination, bytesToRead);
//	std::cerr << "Read " << result << std::endl;
	readContext->completed += result;
	if(readContext->completed >= readContext->blob.GetSize())
		dispatch_source_cancel(readContext->dispatchSource);
}

}
}

#endif // MOLECULAR_GCDASYNCFILELOADER_H
