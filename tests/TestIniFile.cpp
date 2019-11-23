/*	TestIniFile.cpp

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

#include <catch.hpp>
#include <molecular/util/IniFile.h>
#include <molecular/util/MemoryStreamStorage.h>

using namespace molecular;
using namespace molecular::util;

TEST_CASE("TestIniFile")
{
	const char file[] =
			"\n"
			"# Comment  \n"
			"    #Another one\n"
			"[Section1]\n"
			"Key2=ToBeOveridden\n"
			"\tKey1=  123\n"
			"\n"
			"Key2   =asd # Comment\n"
			"line without equal sign  \n"
			"     [  Section2]garbage\n"
			"\tKey3=()/&# COmment\n"
			"   \t  \n"
			"Nothing\n" // Ignored
			"\tKey4  =\t123 asd\n"
			"Empty = \n"
			"a =one char key\n"
			"Key with spaces = blah\n"
			" = empty key   \n" // Ignored
			"\tno newline = at end of file";

	MemoryReadStorage storage(file, sizeof(file));
	IniFile iniFile(storage);

	const IniFile::ValueMap* section = iniFile.GetSection("Section1");
	CHECK(section != NULL);
	CHECK(2 == section->size());

	IniFile::ValueMap::const_iterator it = section->find("Key1");
	CHECK(it != section->end());
	CHECK("123" == (it->second));

	it = section->find("Key2");
	CHECK(it != section->end());
	CHECK("asd" == it->second);

	section = iniFile.GetSection("  Section2");
	CHECK(section != NULL);
	CHECK(6 == section->size());

	it = section->find("Key3");
	CHECK(it != section->end());
	CHECK("()/&" == it->second);

	it = section->find("Key4");
	CHECK(it != section->end());
	CHECK("123 asd" == it->second);

	it = section->find("Nothing");
	CHECK(it == section->end());

	it = section->find("Empty");
	CHECK(it != section->end());
	CHECK("" == it->second);

	it = section->find("Key with spaces");
	CHECK(it != section->end());
	CHECK("blah" == it->second);

	it = section->find("a");
	CHECK(it != section->end());
	CHECK("one char key" == it->second);

	it = section->find("no newline");
	CHECK(it != section->end());
	CHECK("at end of file" == it->second);
}

