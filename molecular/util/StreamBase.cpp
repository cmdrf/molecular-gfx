/*	StreamBase.cpp
	Copyright 2008-2010 Fabian Herb
*/

#include "StreamBase.h"

void WriteStreamBase::Write(const int64_t value, const AttributeInfo *info)
{
	Write(*reinterpret_cast<const uint64_t*>(&value), info);
}


void WriteStreamBase::Write(const int value, const int precision, const AttributeInfo* info)
{
	if(precision > 16)
		Write((const uint32_t)value, info);
	else if(precision > 8)
		Write((const uint16_t)value, info);
	else
		Write((const uint8_t)value, info);
}

void WriteStreamBase::Write(const float value, const AttributeInfo* info)
{
	Write(*reinterpret_cast<const uint32_t*>(&value), info);
}

void WriteStreamBase::Write(const double value, const AttributeInfo* info)
{
	Write(*reinterpret_cast<const uint64_t*>(&value), info);
}

void WriteStreamBase::Write(const bool value, const AttributeInfo* info)
{
	if(value)
		Write(uint8_t(255), info);
	else
		Write(uint8_t(0), info);
}

void WriteStreamBase::Write(const std::string& value, const AttributeInfo* info)
{
	Write(value.c_str(), value.size() + 1, info);
}


void WriteStreamBase::Write(const void* ptr, const size_t size, const AttributeInfo* /*info*/)
{
	for(size_t i = 0; i < size; i++)
		Write((uint8_t*)ptr + i);
}


void WriteStreamBase::Write(const int32_t value, const AttributeInfo* info)
{
	Write(*reinterpret_cast<const uint32_t*>(&value), info);
}

void WriteStreamBase::Write(const int16_t value, const AttributeInfo* info)
{
	Write(*reinterpret_cast<const uint16_t*>(&value), info);
}

void WriteStreamBase::Write(const int8_t value, const AttributeInfo* info)
{
	Write(*reinterpret_cast<const uint8_t*>(&value), info);
}


void ReadStreamBase::Read(int& value, const int precision, const AttributeInfo* /*info*/)
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

void ReadStreamBase::Read(float& value, const AttributeInfo* /*info*/)
{
	Read(reinterpret_cast<int32_t&>(value));
}

void ReadStreamBase::Read(double& value, const AttributeInfo* /*info*/)
{
	Read(reinterpret_cast<uint64_t&>(value));
}

void ReadStreamBase::Read(bool& value, const AttributeInfo* /*info*/)
{
	int8_t valueChar;
	Read(valueChar);
	if(valueChar)
		value = true;
	else
		value = false;
}

void ReadStreamBase::Read(std::string& value, const AttributeInfo* /*info*/)
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

void ReadStreamBase::Read(void* ptr, const size_t size, const AttributeInfo* /*info*/)
{
	for(size_t i = 0; i < size; i++)
		Read(((uint8_t*)ptr)[i]);
}


