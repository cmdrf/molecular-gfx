/*	TestPlaneSet.cpp

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

#include <array>
#include <molecular/util/PlaneSet.h>
#include <catch.hpp>
#include <molecular/testbed/Matchers.h>

using namespace molecular;
using namespace molecular::util;

TEST_CASE("TestPlaneSetClipPolygon")
{
	std::array<Vector3, 4> inPoly = {{Vector3(1, 1, 0), Vector3(-1, 1, 0), Vector3(-1, -1, 0), Vector3(1, -1, 0)}};
	std::array<Plane, 2> planes = {{Plane(Vector3(1, 0, 0), 0), Plane(Vector3(0, 1, 0), 0)}};
	std::array<Vector3, 8> outPoly;
	auto outPolyEnd = PlaneSet::ClipPolygon(planes.begin(), planes.end(), inPoly.begin(), inPoly.end(), outPoly.begin());
	CHECK(4 == outPolyEnd - outPoly.begin());
	CHECK_THAT(outPoly[0], EqualsApprox(Vector3(1, 1, 0)));
	CHECK_THAT(outPoly[1], EqualsApprox(Vector3(0, 1, 0)));
	CHECK_THAT(outPoly[2], EqualsApprox(Vector3(0, 0, 0)));
	CHECK_THAT(outPoly[3], EqualsApprox(Vector3(1, 0, 0)));
}

TEST_CASE("TestPlaneSetCreateCullingPlanes")
{
	std::array<Vector3, 4> inPoly = {{Vector3(1, 1, 0), Vector3(-1, 1, 0), Vector3(-1, -1, 0), Vector3(1, -1, 0)}};
	Vector3 viewPos(0, 0, 1);
	std::array<Plane, 4> planes;
	auto planesEnd = PlaneSet::CreateCullingPlanes(viewPos, inPoly.begin(), inPoly.end(), planes.begin());
	CHECK(planesEnd == planes.end());
	CHECK_THAT(planes[0].GetNormal(), EqualsApprox(Vector3(0, -0.7071f, -0.7071f)));
	CHECK_THAT(planes[1].GetNormal(), EqualsApprox(Vector3( 0.7071f, 0, -0.7071f)));
	CHECK_THAT(planes[2].GetNormal(), EqualsApprox(Vector3(0,  0.7071f, -0.7071f)));
	CHECK_THAT(planes[3].GetNormal(), EqualsApprox(Vector3(-0.7071f, 0, -0.7071f)));
	CHECK(planes[0].GetDistance() == Approx(-0.7071));
	CHECK(planes[1].GetDistance() == Approx(-0.7071));
	CHECK(planes[2].GetDistance() == Approx(-0.7071));
	CHECK(planes[3].GetDistance() == Approx(-0.7071));
}
