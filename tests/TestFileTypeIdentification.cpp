/*	TestFileTypeIdentification.cpp

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
#include <molecular/util/FileTypeIdentification.h>
#include "KtxTestData.h"
#include "TgaTestData.h"
#include "DdsTestData.h"

using namespace molecular;
using namespace molecular::util;

TEST_CASE("TestFileTypeIdentification")
{
	CHECK(FileTypeIdentification::kDds == FileTypeIdentification::Identify(DdsTestData::ddsBct1, sizeof(DdsTestData::ddsBct1)));
	CHECK(FileTypeIdentification::kTga == FileTypeIdentification::Identify(TgaTestData::testData, sizeof(TgaTestData::testData)));
	CHECK(FileTypeIdentification::kTga == FileTypeIdentification::Identify(TgaTestData::testDataRle, sizeof(TgaTestData::testDataRle)));
	CHECK(FileTypeIdentification::kTga == FileTypeIdentification::Identify(TgaTestData::testDataMono, sizeof(TgaTestData::testDataMono)));
	CHECK(FileTypeIdentification::kKtx == FileTypeIdentification::Identify(KtxTestData::exampleImage, sizeof(KtxTestData::exampleImage)));
}
