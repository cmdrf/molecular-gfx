/*	TestPlane.cpp

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
#include <molecular/testbed/Matchers.h>
#include <molecular/util/Plane.h>
#include <array>

using namespace molecular;
using namespace molecular::util;

TEST_CASE("TestPlane")
{
	// Plane through (0,1,0) and (1,0,0), parallel to Z axis
	Plane plane(Vector3(1, 1, 0).Normalized(), 0.5f * sqrtf(2.0));

	// Point:
	CHECK(plane.Check(Vector3(1, 1, 10)) == Plane::kInside);
	CHECK(plane.Check(Vector3(0, 0, -20)) == Plane::kOutside);

	// Sphere:
	CHECK(plane.Check(Vector3(1, 1, 10), 0.2f) == Plane::kInside);
	CHECK(plane.Check(Vector3(0, 0, -20), 0.2f) == Plane::kOutside);
	CHECK(plane.Check(Vector3(1, -1, -2), 1.0) == Plane::kIntersect);

	// Box:
	AxisAlignedBox box1(1, 1, -1, 2, 2, 2);
	CHECK(plane.Check(box1) == Plane::kInside);
	AxisAlignedBox box2(-2, 0, -3, -1, 1, -1);
	CHECK(plane.Check(box2) == Plane::kOutside);
	AxisAlignedBox box3(1, -1, -1, 2, 0, 3);
	CHECK(plane.Check(box3) == Plane::kIntersect);

	Plane plane2(Vector3(-0.0782213f, 0.973193f, -0.216278f), 0.0996125f);
	Plane plane3(Vector3(0.0782229f, -0.973192f, 0.216284f), -2172.12f); // Far
	Plane plane4(Vector3(0.752149f, 0.646178f, -0.129333f), 6.49926f); // Left
	Plane plane5(Vector3(-0.8457f, 0.517749f, -0.129333f), -5.34046f); // Right
	Plane plane6(Vector3(-0.0173536f, 0.215905f, -0.97626f), -1.0669f); // Top
	Plane plane7(Vector3(-0.0487619f, 0.606673f, 0.793454f), 1.88585f); // Bottom
	AxisAlignedBox box4(Vector3(11.4259f, 28.7458f, -0.512952f), Vector3(54.2777f, 102.238f, 14.513f));
	CHECK(Plane::kInside == plane2.Check(box4));
	CHECK(Plane::kInside == plane3.Check(box4));
	CHECK(Plane::kInside == plane4.Check(box4));
	CHECK(Plane::kIntersect == plane5.Check(box4));
	CHECK(Plane::kIntersect == plane6.Check(box4));
	CHECK(Plane::kInside == plane7.Check(box4));
}

TEST_CASE("TestConstructFromThreePoints")
{
	{
		Vector3 p0(0, 0, 0);
		Vector3 p1(1, 0, 1);
		Vector3 p2(0, 1, 1);
		Plane p(p0, p1, p2);
		CHECK_THAT(p.GetNormal(), EqualsApprox(Vector3(-1, -1, 1).Normalized()));
		CHECK(p.GetDistance() == Approx(0.0));
	}
	{
		Vector3 p0(0, 0, 2);
		Vector3 p1(1, 0, 2);
		Vector3 p2(0, 1, 2);
		Plane p(p0, p1, p2);
		CHECK_THAT(p.GetNormal(), EqualsApprox(Vector3(0, 0, 1)));
		CHECK(p.GetDistance() == Approx(2.0));
	}
}

TEST_CASE("TestDecrementAndWrap")
{
	std::array<int, 4> numbers = {{0, 1, 2, 3}};
	auto it = numbers.begin();
	DecrementAndWrap(it, numbers.begin(), numbers.end());
	CHECK(3 == *it);
	DecrementAndWrap(it, numbers.begin(), numbers.end());
	CHECK(2 == *it);
}

TEST_CASE("TestClipPolygon")
{
	Plane plane(Vector3(-1, 0, 0), -1);
	std::array<Vector3, 3> inPoly = {{Vector3(2, 0, 0), Vector3(0, 2, 0), Vector3(0, -2, 0)}};
	std::array<Vector3, 4> outPoly;
	auto outPolyEnd = plane.ClipPolygon(inPoly.begin(), inPoly.end(), outPoly.begin());
	CHECK(4 == outPolyEnd - outPoly.begin());
	CHECK_THAT(outPoly.at(0), EqualsApprox(Vector3(0, 2, 0)));
	CHECK_THAT(outPoly.at(1), EqualsApprox(Vector3(0, -2, 0)));
	CHECK_THAT(outPoly.at(2), EqualsApprox(Vector3(1, -1, 0)));
	CHECK_THAT(outPoly.at(3), EqualsApprox(Vector3(1, 1, 0)));

	Plane p2(Vector3(1, 0, 0), 1);
	outPolyEnd = p2.ClipPolygon(inPoly.begin(), inPoly.end(), outPoly.begin());
	CHECK(3 == outPolyEnd - outPoly.begin());
	CHECK_THAT(outPoly.at(0), EqualsApprox(Vector3(2, 0, 0)));
	CHECK_THAT(outPoly.at(1), EqualsApprox(Vector3(1, 1, 0)));
	CHECK_THAT(outPoly.at(2), EqualsApprox(Vector3(1, -1, 0)));

	// Completely outside:
	Plane p3(Vector3(1, 0, 0), 3);
	outPolyEnd = p3.ClipPolygon(inPoly.begin(), inPoly.end(), outPoly.begin());
	CHECK(0 == outPolyEnd - outPoly.begin());

	// Polygon unchanged:
	Plane p4(Vector3(1, 0, 0), -3);
	outPolyEnd = p4.ClipPolygon(inPoly.begin(), inPoly.end(), outPoly.begin());
	CHECK(3 == outPolyEnd - outPoly.begin());
	CHECK_THAT(outPoly.at(0), EqualsApprox(Vector3(2, 0, 0)));
	CHECK_THAT(outPoly.at(1), EqualsApprox(Vector3(0, 2, 0)));
	CHECK_THAT(outPoly.at(2), EqualsApprox(Vector3(0, -2, 0)));

	// Quad:
	std::array<Vector3, 4> inPoly2 = {{Vector3(1, 1, 0), Vector3(-1, 1, 0), Vector3(-1, -1, 0), Vector3(1, -1, 0)}};
	Plane p5(Vector3(0, 1, 0), 0);
	outPolyEnd = p5.ClipPolygon(inPoly2.begin(), inPoly2.end(), outPoly.begin());
	CHECK(4 == outPolyEnd - outPoly.begin());
	CHECK_THAT(outPoly[0], EqualsApprox(Vector3(1, 1, 0)));
	CHECK_THAT(outPoly[1], EqualsApprox(Vector3(-1, 1, 0)));
	CHECK_THAT(outPoly[2], EqualsApprox(Vector3(-1, 0, 0)));
	CHECK_THAT(outPoly[3], EqualsApprox(Vector3(1, 0, 0)));
}

TEST_CASE("TestPlaneIntersectionPoint")
{
	Plane p(Vector3(1, 0, 0), 1);
	Vector3 p0(-1, 0, 1);
	Vector3 p1(3, 2, 1);
	Vector3 i = p.IntersectionPoint(p0, p1);
	CHECK_THAT(i, EqualsApprox(Vector3(1, 1, 1)));
}
