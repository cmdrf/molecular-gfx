/*	HostStream.h

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

#ifndef MOLECULAR_HOSTSTREAM_H
#define MOLECULAR_HOSTSTREAM_H

#include "ReadStream.h"
#include "StreamStorage.h"

namespace molecular
{
namespace util
{

/// Stream-interface to a ReadStorage that does no endian conversion.
/** @deprecated Most subsystems around molecular now cast file contents to a struct. */
template<class Storage>
class HostReadStream : public ReadStream
{
public:
	HostReadStream(Storage& storage) : mStorage(storage){}
	void Read(uint8_t&	value) override;
	void Read(uint16_t&	value) override;
	void Read(uint32_t&	value) override;
	void Read(uint64_t&	value) override;
	void Read(void*	ptr, const size_t size) override;

	using ReadStream::Read;

private:
	Storage& mStorage;
};

/*****************************************************************************/

template<class Storage>
void HostReadStream<Storage>::Read(uint8_t& value)
{
	mStorage.Read(&value, 1);
}

template<class Storage>
void HostReadStream<Storage>::Read(uint16_t& value)
{
	mStorage.Read(&value, 2);
}

template<class Storage>
void HostReadStream<Storage>::Read(uint32_t& value)
{
	mStorage.Read(&value, 4);
}

template<class Storage>
void HostReadStream<Storage>::Read(uint64_t& value)
{
	mStorage.Read(&value, 8);
}

template<class Storage>
void HostReadStream<Storage>::Read(void* ptr, const size_t size)
{
	mStorage.Read(ptr, size);
}

}
}

#endif
