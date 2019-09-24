/*	TestStringStore.cpp

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
#include <molecular/util/StringStore.h>
#include <molecular/util/Hash.h>
#include <iostream>

using Catch::Matchers::Equals;
using namespace molecular;

TEST_CASE("TestStringStore")
{
	const uint8_t testData[] = {
		0xc0, 0x0f, 0x7a, 0x80, // magic
		0x04, 0x00, 0x00, 0x00, // count
		29, 0x00, 0x00, 0x00, // stringListSize
		0x00, 0x00, 0x00, 0x00, // reserved
		0xcd, 0x53, 0x5a, 0x61, 0x00, 0x00, 0x00, 0x00,
		0x28, 0x78, 0xbc, 0x34, 0x04, 0x00, 0x00, 0x00,
		0xdb, 0x46, 0xeb, 0xdf, 14, 0x00, 0x00, 0x00,
		0x0c, 0xe9, 0x89, 0x8e, 23, 0x00, 0x00, 0x00,
		'b', 'l', 'a', 0, // 4
		's', 'a', 'p', 'p', 'e', 'r', 'l', 'o', 't', 0, // 10
		't', 'u', 'e', 'd', 'e', 'l', 'u', 'e', 0, // 9
		'l', 'a', 'b', 'e', 'r', 0 // 6
	};

	StringStore store(testData, sizeof(testData));

	CHECK_THAT(store.GetString("bla"_H), Equals("bla"));
	CHECK_THAT(store.GetString("sapperlot"_H), Equals("sapperlot"));
	CHECK_THAT(store.GetString("tuedelue"_H), Equals("tuedelue"));
	CHECK_THAT(store.GetString("laber"_H), Equals("laber"));
}
