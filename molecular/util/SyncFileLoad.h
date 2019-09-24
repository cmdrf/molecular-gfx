/*	SyncFileLoad.h

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

#ifndef MOLECULAR_SYNCFILELOAD_H
#define MOLECULAR_SYNCFILELOAD_H

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
