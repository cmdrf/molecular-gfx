/*	TestTgaFile.cpp

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
#include <molecular/util/TgaFile.h>
#include <molecular/util/MemoryStreamStorage.h>
#include "TgaTestData.h"

using namespace molecular;
using namespace molecular::util;

TEST_CASE("TestTgaFile")
{
	TgaFile2 file(TgaTestData::testData, 812);
	CHECK(16 == file.GetWidth());
	CHECK(16 == file.GetHeight());
	CHECK(false == file.IsUpsideDown());
	CHECK(PF_B8G8R8 == file.GetFormat());
}

TEST_CASE("TestTgaFileMono")
{
	TgaFile2 file(TgaTestData::testDataMono, sizeof(TgaTestData::testDataMono));

	CHECK(16 == file.GetHeight());
	CHECK(16 == file.GetHeight());
	CHECK(false == file.IsUpsideDown());
	CHECK(PF_L8 == file.GetFormat());
}
