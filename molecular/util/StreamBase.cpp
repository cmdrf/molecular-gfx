/*	StreamBase.cpp

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

#include "StreamBase.h"

namespace molecular
{

void WriteStreamBase::Write(const int64_t value)
{
	Write(*reinterpret_cast<const uint64_t*>(&value));
}


void WriteStreamBase::Write(const int value, const int precision)
{
	if(precision > 16)
		Write((const uint32_t)value);
	else if(precision > 8)
		Write((const uint16_t)value);
	else
		Write((const uint8_t)value);
}

void WriteStreamBase::Write(const float value)
{
	Write(*reinterpret_cast<const uint32_t*>(&value));
}

void WriteStreamBase::Write(const double value)
{
	Write(*reinterpret_cast<const uint64_t*>(&value));
}

void WriteStreamBase::Write(const bool value)
{
	if(value)
		Write(uint8_t(255));
	else
		Write(uint8_t(0));
}

void WriteStreamBase::Write(const std::string& value)
{
	Write(value.c_str(), value.size() + 1);
}


void WriteStreamBase::Write(const void* ptr, const size_t size)
{
	for(size_t i = 0; i < size; i++)
		Write((uint8_t*)ptr + i);
}


void WriteStreamBase::Write(const int32_t value)
{
	Write(*reinterpret_cast<const uint32_t*>(&value));
}

void WriteStreamBase::Write(const int16_t value)
{
	Write(*reinterpret_cast<const uint16_t*>(&value));
}

void WriteStreamBase::Write(const int8_t value)
{
	Write(*reinterpret_cast<const uint8_t*>(&value));
}


void ReadStreamBase::Read(int& value, const int precision)
{
	if(precision > 16)
	{
		int32_t longVal;
		Read(longVal);
		value = longVal;
	}
	else if(precision > 8)
	{
		int16_t	shortVal;
		Read(shortVal);
		value = shortVal;
	}
	else
	{
		int8_t charVal;
		Read(charVal);
		value = charVal;
	}
}

void ReadStreamBase::Read(float& value)
{
	Read(reinterpret_cast<int32_t&>(value));
}

void ReadStreamBase::Read(double& value)
{
	Read(reinterpret_cast<uint64_t&>(value));
}

void ReadStreamBase::Read(bool& value)
{
	int8_t valueChar;
	Read(valueChar);
	if(valueChar)
		value = true;
	else
		value = false;
}

void ReadStreamBase::Read(std::string& value)
{
	int8_t byte;
	value.clear();
	while(true)
	{
		Read(byte);
		if(byte == 0)
			break;
		value.push_back(byte);
	}
}

void ReadStreamBase::Read(void* ptr, const size_t size)
{
	for(size_t i = 0; i < size; i++)
		Read(((uint8_t*)ptr)[i]);
}

}
