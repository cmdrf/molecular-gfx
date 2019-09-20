/*	TestFrustum.cpp

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
#include <molecular/util/Frustum.h>
#include <molecular/util/Math.h>
#include <molecular/util/Matrix4.h>

using namespace molecular;
using namespace molecular::util;

TEST_CASE("TestFrustum")
{
	// Projection with 90 degrees field of view:
	Matrix4 projection = Matrix4::ProjectionPerspective(0.5f * Math::kPi_f, 1, 0.5, 10);
	// Rotate it to cover the quadrant +X +Y:
	Matrix<4,4> view = Matrix4::RotationX(-0.5f * Math::kPi_f) * Matrix4::RotationZ(0.25f * Math::kPi_f);
	Matrix<4,4> viewProjection = projection * view;
	Frustum frustum(viewProjection);

	CHECK(frustum.GetPlane(Frustum::kNear).GetNormal().Length() == Approx(1.0));
	CHECK(frustum.GetPlane(Frustum::kFar).GetNormal().Length() == Approx(1.0));
	CHECK(frustum.GetPlane(Frustum::kLeft).GetNormal().Length() == Approx(1.0));
	CHECK(frustum.GetPlane(Frustum::kRight).GetNormal().Length() == Approx(1.0));
	CHECK(frustum.GetPlane(Frustum::kTop).GetNormal().Length() == Approx(1.0));
	CHECK(frustum.GetPlane(Frustum::kBottom).GetNormal().Length() == Approx(1.0));

	// Point:
	CHECK(frustum.Check(Vector3(1.0f, 1.0f, 0.0f)) == Plane::kInside);
	CHECK(frustum.Check(Vector3(1.0f, -1.0f, 0.0f)) == Plane::kOutside);
	CHECK(frustum.Check(Vector3(-0.1f, 8.1f, 0.0f)) == Plane::kOutside);
	CHECK(frustum.Check(Vector3(0.1f, 0.1f, 0.0f)) == Plane::kOutside);

	// Sphere:
	CHECK(frustum.Check(Vector3(2.0f, 2.0f, 0.0f), 0.5f) == Plane::kInside);
	CHECK(frustum.Check(Vector3(-0.1f, 2.1f, 0.0f), 0.2f) == Plane::kIntersect);
	CHECK(frustum.Check(Vector3(0.1f, 0.1f, 0.0f), 0.2f) == Plane::kOutside);
	CHECK(frustum.Check(Vector3(2.0f, -1.0f, 0.0f), 0.2f) == Plane::kOutside);

	// Box:
	CHECK(frustum.Check(AxisAlignedBox(2,2,-1,3,3,1)) == Plane::kInside);
	CHECK(frustum.Check(AxisAlignedBox(8,8,-3,9,9,-2)) == Plane::kOutside);
	CHECK(frustum.Check(AxisAlignedBox(-1,2,1,1,3,2)) == Plane::kIntersect);
	CHECK(frustum.Check(AxisAlignedBox(-1,-1,-1,1,1,1)) == Plane::kIntersect);
	CHECK(frustum.Check(AxisAlignedBox(2,2,1,3,3,5)) == Plane::kIntersect);
	CHECK(frustum.Check(AxisAlignedBox(-1,-1,-10,10,10,10)) == Plane::kIntersect);
}


TEST_CASE("TestFrustumFromMatrix")
{
	Frustum frustum(Matrix4::Identity());
	CHECK(frustum.GetPlane(Frustum::kNear).GetNormal().Distance(Vector3(0, 0, 1)) == Approx(0.0));
	CHECK(frustum.GetPlane(Frustum::kFar).GetNormal().Distance(Vector3(0, 0, -1)) == Approx(0.0));
	CHECK(frustum.GetPlane(Frustum::kLeft).GetNormal().Distance(Vector3(1, 0, 0)) == Approx(0.0));
	CHECK(frustum.GetPlane(Frustum::kRight).GetNormal().Distance(Vector3(-1, 0, 0)) == Approx(0.0));
	CHECK(frustum.GetPlane(Frustum::kTop).GetNormal().Distance(Vector3(0, -1, 0)) == Approx(0.0));
	CHECK(frustum.GetPlane(Frustum::kBottom).GetNormal().Distance(Vector3(0, 1, 0)) == Approx(0.0));

	CHECK(frustum.GetPlane(Frustum::kNear).GetDistance() == Approx(-1.0));
	CHECK(frustum.GetPlane(Frustum::kFar).GetDistance() == Approx(-1.0));
	CHECK(frustum.GetPlane(Frustum::kLeft).GetDistance() == Approx(-1.0));
	CHECK(frustum.GetPlane(Frustum::kRight).GetDistance() == Approx(-1.0));
	CHECK(frustum.GetPlane(Frustum::kTop).GetDistance() == Approx(-1.0));
	CHECK(frustum.GetPlane(Frustum::kBottom).GetDistance() == Approx(-1.0));

	frustum = Frustum(Matrix4::ProjectionPerspective(0.5f * Math::kPi_f, 1, 1, 100));
	CHECK(frustum.GetPlane(Frustum::kNear).GetNormal().Distance(Vector3(0, 0, -1)) == Approx(0.0));
	CHECK(frustum.GetPlane(Frustum::kFar).GetNormal().Distance(Vector3(0, 0, 1)) == Approx(0.0));
	CHECK(frustum.GetPlane(Frustum::kNear).GetDistance() == Approx(1.0));
	CHECK(frustum.GetPlane(Frustum::kFar).GetDistance() == Approx(-100.0));
}
