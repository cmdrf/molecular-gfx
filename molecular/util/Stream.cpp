/*	Stream.cpp
	Copyright 2008-2010 Fabian Herb
*/

#include "Stream.h"
#include <molecular/util/Vector3.h>
#include <molecular/util/Quaternion.h>

namespace molecular
{

void WriteStream::Write(const Vector3& value)
{	
	Write(value.X());
	Write(value.Y());
	Write(value.Z());
	
}

void WriteStream::Write(const Quaternion& value)
{
	Write(value.q[0]);
	Write(value.q[1]);
	Write(value.q[2]);
	Write(value.q[3]);
}

void ReadStream::Read(Vector3& value)
{
	Read(value[0]);
	Read(value[1]);
	Read(value[2]);
}

void ReadStream::Read(Quaternion& value)
{
	Read(value.q[0]);
	Read(value.q[1]);
	Read(value.q[2]);
	Read(value.q[3]);
}

void WriteStream::WriteListBegin(const uint16_t numItems)
{
	Write(numItems);
}

void WriteStream::Write(const float values[], size_t count)
{
	for(size_t i = 0; i < count; ++i)
		Write(values[i]);
}

void ReadStream::ReadListBegin(uint16_t& numItems)
{
	Read(numItems);
}

void ReadStream::Read(float values[], size_t count)
{
	for(size_t i = 0; i < count; ++i)
		Read(values[i]);
}

}
