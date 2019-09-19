/*	IniFile.cpp
	Copyright 2013-2016 Fabian Herb

	This file is part of Molecular Engine.
*/

#include "IniFile.h"
#include "MemoryStreamStorage.h"

IniFile::IniFile(const void* data, size_t size)
{
	Load(data, size);
}

void IniFile::Load(const void* data, size_t size)
{
	MemoryReadStorage storage(data, size);
	Load(storage);
}
