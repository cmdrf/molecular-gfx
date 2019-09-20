/*	IniFile.h

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

#ifndef MOLECULAR_INIFILE_H
#define MOLECULAR_INIFILE_H

#include "TextStream.h"
#include <unordered_map>
#include <string>

namespace molecular
{

/// ini file parser
class IniFile
{
public:
	typedef std::unordered_map<std::string, std::string> ValueMap;
	typedef std::unordered_map<std::string, ValueMap> SectionMap;

	IniFile() = default;
	IniFile(const void* data, size_t size);

	template<class Storage>
	IniFile(Storage& storage);

	void Load(const void* data, size_t size);

	template<class Storage>
	void Load(Storage& storage);

	const ValueMap* GetSection(const std::string& section) const
	{
		SectionMap::const_iterator it = mSectionMap.find(section);
		if(it != mSectionMap.end())
			return &it->second;
		else
			return nullptr;
	}

	const SectionMap& GetAllSections() const {return mSectionMap;}

	const ValueMap& at(const std::string& section) const
	{
		auto it = mSectionMap.find(section);
		if(it == mSectionMap.end())
			throw std::runtime_error("IniFile: No section named \"" + section + "\" found");
		return it->second;
	}

private:
	SectionMap mSectionMap;
};

/*****************************************************************************/

template<class Storage>
IniFile::IniFile(Storage& storage)
{
	Load(storage);
}

template<class Storage>
void IniFile::Load(Storage& storage)
{
	TextReadStream<Storage> stream(storage);
	char* line;
	ValueMap* currentSection = nullptr;
	while((line = stream.GetNextLine()))
	{
		while(line[0] > 0 && isspace(line[0]))
			line++;

		if(line[0] == 0 || line[0] == '#')
			continue;

		if(line[0] == '[')
		{
			line++;
			char* lineEnd = line;
			while(*lineEnd && *lineEnd != ']')
				lineEnd++;
			*lineEnd = 0;
			currentSection = &mSectionMap[line];
		}
		else if(currentSection)
		{
			char* keyEnd = line;
			// Advance to '='
			while(*keyEnd && *keyEnd != '=')
				keyEnd++;

			if(*keyEnd != '=')
				continue; // No equal sign in line

			// Strip whitespace at end:
			do
			{
				keyEnd--;
			}while(keyEnd > line && isspace(*keyEnd));
			keyEnd++;

			// Skip empty keys:
			if(!*keyEnd || keyEnd == line)
				continue;

			// Advance over whitespace and '=' to value:
			char* valueBegin = keyEnd;
			while(*valueBegin && (isspace(*valueBegin) || *valueBegin == '='))
				valueBegin++;

			// Advance until line end or comment:
			char* valueEnd = valueBegin;
			while(*valueEnd && *valueEnd != '#')
				valueEnd++;

			// Strip whitespace at end:
			do
			{
				valueEnd--;
			}while(valueEnd > valueBegin && isspace(*valueEnd));
			valueEnd++;

			// Terminate strings:
			*keyEnd = 0;
			*valueEnd = 0;

			(*currentSection)[line] = valueBegin;
		}
	}
}
}

#endif // MOLECULAR_INIFILE_H
