/*	TestBox.cpp

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

#include <cmath>

#include <catch.hpp>
#include <molecular/util/Box.h>
#include <molecular/util/Math.h>
#include <molecular/util/AxisAlignedBox.h>

using namespace molecular;
using namespace molecular::util;

TEST_CASE("TestBoxConstructTransform")
{
	Box box(Vector3(2, 3, 4), Vector3(10, 12, 7));
	Vector3 corner000 = box.GetCorner(false, false, false);
	CHECK(corner000[0] == Approx(9));
	CHECK(corner000[1] == Approx(10.5));
	CHECK(corner000[2] == Approx(5));

	box.Transform(Matrix4::RotationZ(Math::kPi_f));
	corner000 = box.GetCorner(false, false, false);
	CHECK(corner000[0] == Approx(-9));
	CHECK(corner000[1] == Approx(-10.5));
	CHECK(corner000[2] == Approx(5));

	box.Transform(Matrix4::RotationY(Math::kPi_f));
	corner000 = box.GetCorner(false, false, false);
	CHECK(corner000[0] == Approx(9));
	CHECK(corner000[1] == Approx(-10.5));
	CHECK(corner000[2] == Approx(-5));
}

TEST_CASE("TestBoxConstructAabb")
{
	AxisAlignedBox aabb(Vector3(-1, -1, -1), Vector3(1, 1, 1));
	Box box(aabb);
	Vector3 corner000 = box.GetCorner(false, false, false);
	CHECK(corner000[0] == Approx(-1));
	CHECK(corner000[1] == Approx(-1));
	CHECK(corner000[2] == Approx(-1));
}
