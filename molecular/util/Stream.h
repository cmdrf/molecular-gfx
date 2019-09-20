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
#include "AttributeInfo.h"
#include <molecular/util/Vector3.h>
#include <vector>

namespace molecular
{

class Quaternion;

/// Abstract base class for data storage streams
/** This has extensions over StreamBase specific to Molecular Engine.
	@deprecated Data should be written without endianess conversion etc. */
class WriteStream : public WriteStreamBase
{
public:
	
	virtual void Write(const Vector3& value, const AttributeInfo* info = nullptr);
	virtual void Write(const Quaternion& value, const AttributeInfo* info = nullptr);

	// Make overloaded inherited functions visible:
	using WriteStreamBase::Write;

	/// This allows passing a string instead of an AttributeInfo object to most Write() functions
	template<typename T>
	void Write(const T& value, const char* description)
	{
		AttributeInfo info(description);
		Write(value, &info);
	}

	virtual	void WriteListBegin(const uint16_t numItems, const AttributeInfo* info = nullptr);
	void WriteListBegin(const uint16_t numItems, const char* description);
	virtual	void WriteItemBegin(){}
	virtual	void WriteListEnd(){}
	virtual	void WriteItemEnd(){}

	/// Convenience Write method for vectors of structs
	/** Calls a Write method in every struct. */
	template<class T>
	void WriteStructVector(const typename std::vector<T>& elements, const char* description);

	/// Write array of float values
	virtual void Write(const float values[], size_t count, const AttributeInfo* info);
	void Write(const float values[], size_t count, const char* description)
	{
		AttributeInfo info(description);
		Write(values, count, &info);
	}
};

class ReadStream : public ReadStreamBase
{
public:
	virtual	void Read(Vector3&	value, const AttributeInfo* info = nullptr);
	virtual	void Read(Quaternion&	value, const AttributeInfo* info = nullptr);

	// Make overloaded inherited functions visible:
	using ReadStreamBase::Read;
	
	/// This allow passing a string instead of an AttributeInfo object to most Read() functions
	template<typename T>
	void Read(T& value, const char* description)
	{
		AttributeInfo info(description);
		Read(value, &info);
	}

	virtual	void ReadListBegin(uint16_t& numItems, const AttributeInfo* info = nullptr);
	void ReadListBegin(uint16_t& numItems, const char* description);

	virtual	void ReadItemBegin() {}
	virtual	void ReadListEnd() {}
	virtual	void ReadItemEnd() {}

	template<class T>
	void ReadStructVector(std::vector<T>& elements, const char* description);

	/// Read array of float values
	virtual void Read(float values[], size_t count, const AttributeInfo* info);
	void Read(float values[], size_t count, const char* description)
	{
		AttributeInfo info(description);
		Read(values, count, &info);
	}

};

/*****************************************************************************/

template<class T>
void WriteStream::WriteStructVector(const std::vector<T>& elements, const char* description)
{
	WriteListBegin(uint16_t(elements.size()), description);
	for(size_t i = 0; i < elements.size(); ++i)
	{
		WriteItemBegin();
		elements[i].Write(*this);
		WriteItemEnd();
	}
	WriteListEnd();
}

template<class T>
void ReadStream::ReadStructVector(std::vector<T>& elements, const char* description)
{
	uint16_t numItems;
	ReadListBegin(numItems, description);
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
