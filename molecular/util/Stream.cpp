/*	Stream.cpp
	Copyright 2008-2010 Fabian Herb
*/

#include "Stream.h"
#include <molecular/util/Vector3.h>
#include <molecular/util/Quaternion.h>

namespace molecular
{

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
