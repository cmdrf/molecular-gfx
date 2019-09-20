/*	AttributeInfo.cpp
	Copyright 2008-2014 Fabian Herb

	This file is part of Molecular Engine.
*/

#include "AttributeInfo.h"
#include <string.h>

const char* AttributeInfo::FindOption(int key) const
{
	for(size_t i = 0; i < mOptionCount; ++i)
	{
		if(mOptions[i].first == key)
			return mOptions[i].second;
	}
	return nullptr;
}

int AttributeInfo::FindOption(const char* name) const
{
	for(size_t i = 0; i < mOptionCount; ++i)
	{
		if(strcmp(mOptions[i].second, name) == 0)
			return mOptions[i].first;
	}
	return -1;
}
