/*	TestDdsFile.cpp

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
#include <molecular/util/DdsFile.h>
#include <molecular/util/MemoryStreamStorage.h>
#include "DdsTestData.h"

using namespace molecular;

struct DdsTest
{
	PixelFormat Test(const uint8_t* data, size_t dataSize, size_t expectedImageSize)
	{
//		MemoryReadStorage storage(data, dataSize);
		util::DdsFile file(static_cast<const void*>(data), dataSize);
		CHECK(1 == file.GetNumFaces());
		CHECK(1 == file.GetNumMipmapLevels());

		unsigned int width, height;
		size_t size;
		const void* pointer = file.GetSingleImage(0, 0, width, height, size);
		CHECK(8 == width);
		CHECK(8 == height);
		CHECK(expectedImageSize == size);
		CHECK(std::equal(data + 128, data + 128 + expectedImageSize, reinterpret_cast<const uint8_t*>(pointer)));
		return file.GetFormat();
	}
};

TEST_CASE_METHOD(DdsTest, "TestDxt1")
{
	CHECK(PF_RGBA_DXT1 == Test(DdsTestData::ddsDxt1, 160, 32));
}

TEST_CASE_METHOD(DdsTest, "TestRgb")
{
	CHECK(PF_R8G8B8 == Test(DdsTestData::ddsRgb, 320, 192));
}
