/*	TestKtxFile.cpp

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
#include <molecular/util/KtxFile.h>
#include "KtxTestData.h"

using namespace molecular;

TEST_CASE("TestKtxFile")
{
	KtxFile ktxFile(KtxTestData::exampleImage, sizeof(KtxTestData::exampleImage));
	CHECK(0 == ktxFile.GetGlType());
	CHECK(1 == ktxFile.GetGlTypeSize());
	auto level0 = ktxFile.GetImageData(0);
	CHECK(512 == level0.second);
	CHECK(0xdad8d8d8 == static_cast<const uint32_t*>(level0.first)[0]);
}

TEST_CASE("TestKtxFileMipMaps")
{
	KtxFile ktxFile(KtxTestData::exampleImageMipMaps, sizeof(KtxTestData::exampleImageMipMaps));
	CHECK(5 == ktxFile.GetNumberOfMipmapLevels());
	auto level0 = ktxFile.GetImageData(0);
	auto level1 = ktxFile.GetImageData(1);
	auto level2 = ktxFile.GetImageData(2);
	auto level3 = ktxFile.GetImageData(3);
	auto level4 = ktxFile.GetImageData(4);
	CHECK(160 == level0.second);
	CHECK(48 == level1.second);
	CHECK(8 == level2.second);
	CHECK(8 == level3.second);
	CHECK(8 == level4.second);
	CHECK(0x4b9777eb == static_cast<const uint32_t*>(level0.first)[0]);
	CHECK(0x8e750c4c == static_cast<const uint32_t*>(level1.first)[0]);
	CHECK(0x8b99a3f9 == static_cast<const uint32_t*>(level2.first)[0]);
	CHECK(0x32dd0565 == static_cast<const uint32_t*>(level3.first)[0]);
	CHECK(0xdef3a25d == static_cast<const uint32_t*>(level4.first)[0]);
}
