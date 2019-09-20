/*	AttributeInfo.h
	Copyright 2008-2014 Fabian Herb

	This file is part of DeiDreh.
*/

#ifndef ATTRIBUTEINFO_H
#define ATTRIBUTEINFO_H

#include <string>
#include "Types.h"
#include <vector>

/// Property attribute information.
/** When properties write out their data, they should provide a pointer to an AttributeInfo
	for each attribute, so the editor can display a name etc.
	@deprecated */
class AttributeInfo
{
public:
	typedef std::pair<int, const char*> OptionType;
	inline AttributeInfo(const char* name, const char* description = "") :
		mName(name),
		mDescription(description),
		mOptions(nullptr),
		mOptionCount(0),
		mEditable(true)
	{
	}

	inline AttributeInfo(const char* name, const OptionType* options, size_t optionCount) :
		mName(name),
		mDescription(""),
		mOptions(options),
		mOptionCount(optionCount),
		mEditable(true)
	{
	}

	const char* FindOption(int key) const;
	int FindOption(const char* name) const;
	
	inline const char* GetName() const {return mName;}
	inline const char* GetDescription() const {return mDescription;}
	inline bool IsEditable() const {return mEditable;}

private:
	const char* mName;
	const char* mDescription;
	const OptionType* mOptions;
	size_t mOptionCount;
	bool mEditable;
};

#endif
