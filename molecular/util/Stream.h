/*	Stream.h

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

#ifndef MOLECULAR_STREAM_H
#define MOLECULAR_STREAM_H

#include "StreamBase.h"
#include <molecular/util/Vector3.h>
#include <vector>

namespace molecular
{

class Quaternion;

/// Abstract base class for data storage streams
/** This has extensions over StreamBase specific to Molecular Engine.
	@deprecated Data should be read without endianess conversion etc. */
class ReadStream : public ReadStreamBase
{
public:
	virtual	void Read(Vector3& value);
	virtual	void Read(Quaternion& value);

	// Make overloaded inherited functions visible:
	using ReadStreamBase::Read;

	virtual	void ReadListBegin(uint16_t& numItems);

	virtual	void ReadItemBegin() {}
	virtual	void ReadListEnd() {}
	virtual	void ReadItemEnd() {}

	template<class T>
	void ReadStructVector(std::vector<T>& elements);

	/// Read array of float values
	virtual void Read(float values[], size_t count);
};

/*****************************************************************************/

template<class T>
void ReadStream::ReadStructVector(std::vector<T>& elements)
{
	uint16_t numItems;
	ReadListBegin(numItems);
	elements.resize(numItems);
	for(size_t i = 0; i < elements.size(); ++i)
	{
		ReadItemBegin();
		elements[i].Read(*this);
		ReadItemEnd();
	}
	ReadListEnd();
}

}

#endif
