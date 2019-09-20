/*	Stream.cpp
	Copyright 2008-2010 Fabian Herb
*/

#include "Stream.h"
#include <molecular/util/Vector3.h>
#include <molecular/util/Quaternion.h>

namespace molecular
{

void WriteStream::Write(const Vector3& value, const AttributeInfo* info)
{	
	Write(value.X(), info);
	Write(value.Y(), info);
	Write(value.Z(), info);
	
}

void WriteStream::Write(const Quaternion& value, const AttributeInfo* info)
{
	Write(value.q[0], info);
	Write(value.q[1], info);
	Write(value.q[2], info);
	Write(value.q[3], info);
}

void ReadStream::Read(Vector3& value, const AttributeInfo* info)
{
	Read(value[0], info);
	Read(value[1], info);
	Read(value[2], info);
}

void ReadStream::Read(Quaternion& value, const AttributeInfo* info)
{
	Read(value.q[0], info);
	Read(value.q[1], info);
	Read(value.q[2], info);
	Read(value.q[3], info);
}

void WriteStream::WriteListBegin(const uint16_t numItems, const AttributeInfo* info)
{
	Write(numItems, info);
}

void WriteStream::WriteListBegin(const uint16_t numItems, const char* name)
{
	AttributeInfo info(name);
	WriteListBegin(numItems, &info);
}

void WriteStream::Write(const float values[], size_t count, const AttributeInfo* info)
{
	for(size_t i = 0; i < count; ++i)
		Write(values[i]);
}

void ReadStream::ReadListBegin(uint16_t& numItems, const AttributeInfo* info)
{
	Read(numItems, info);
}

void ReadStream::ReadListBegin(uint16_t& numItems, const char* name)
{
	AttributeInfo info(name);
	ReadListBegin(numItems, &info);
}

void ReadStream::Read(float values[], size_t count, const AttributeInfo* info)
{
	for(size_t i = 0; i < count; ++i)
		Read(values[i]);
}

}
