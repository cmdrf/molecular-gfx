/*	TestQuaternion.cpp

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
#include <molecular/util/Quaternion.h>
#include <molecular/util/Matrix3.h>
#include <molecular/util/Math.h>

using namespace molecular;
using namespace molecular::testbed;
using namespace molecular::util;
using namespace molecular::Math;

TEST_CASE("TestQuaternionRotation")
{
	Quaternion x180 = Quaternion::RotationX(kPi_f);
	CHECK_THAT(x180, EqualsApprox(Quaternion(0, 1, 0, 0), 0.001, 0.000001));

	Quaternion y180 = Quaternion::RotationY(kPi_f);
	CHECK_THAT(y180, EqualsApprox(Quaternion(0, 0, 1, 0), 0.001, 0.000001));

	Quaternion z180 = Quaternion::RotationZ(kPi_f);
	CHECK_THAT(z180, EqualsApprox(Quaternion(0, 0, 0, 1), 0.001, 0.000001));

	Quaternion x90 = Quaternion::RotationX(kPi_f * 0.5f);
	CHECK_THAT(x90, EqualsApprox(Quaternion(0.7071f, 0.7071f, 0, 0)));

	Quaternion y90 = Quaternion::RotationY(kPi_f * 0.5f);
	CHECK_THAT(y90, EqualsApprox(Quaternion(0.7071f, 0, 0.7071f, 0)));

	Quaternion z90 = Quaternion::RotationZ(kPi_f * 0.5f);
	CHECK_THAT(z90, EqualsApprox(Quaternion(0.7071f, 0, 0, 0.7071f)));

	Quaternion x90n = Quaternion::RotationX(kPi_f * -0.5f);
	CHECK_THAT(x90n, EqualsApprox(Quaternion(0.7071f, -0.7071f, 0, 0)));

	Quaternion y90n = Quaternion::RotationY(kPi_f * -0.5f);
	CHECK_THAT(y90n, EqualsApprox(Quaternion(0.7071f, 0, -0.7071f, 0)));

	Quaternion z90n = Quaternion::RotationZ(kPi_f * -0.5f);
	CHECK_THAT(z90n, EqualsApprox(Quaternion(0.7071f, 0, 0, -0.7071f)));
}

TEST_CASE("TestQuaternionRotationOrder")
{
	Quaternion qX = Quaternion::RotationX(0.3f);
	Quaternion qY = Quaternion::RotationY(1.1f);
	Quaternion qZ = Quaternion::RotationZ(0.8f);
	Matrix3 mX = Matrix3::RotationX(0.3f);
	Matrix3 mY = Matrix3::RotationY(1.1f);
	Matrix3 mZ = Matrix3::RotationZ(0.8f);
	Quaternion qResult = qX * qY * qZ;
	Matrix3 mResult = mX * mY * mZ;
	CHECK_THAT(Matrix3(qResult), EqualsApprox(mResult));
}

TEST_CASE("TestQuaternionSlerp")
{
	Quaternion q1(0.0f,  0.6f, 0.8f,  0.0f);
	Quaternion q2(0.0f, -0.8f, 0.0f, -0.6f);
	CHECK_THAT(Quaternion::Slerp(q1, q2, 0), EqualsApprox(Quaternion(0, 0.6f, 0.8f, 0)));
	CHECK_THAT(Quaternion::Slerp(q1, q2, 0.1f), EqualsApprox(Quaternion(0, 0.46714f, 0.872923f, -0.140664f)));
	CHECK_THAT(Quaternion::Slerp(q1, q2, 0.2f), EqualsApprox(Quaternion(0,  0.314307f,  0.908523f, -0.275314f)));
	CHECK_THAT(Quaternion::Slerp(q1, q2, 0.3f), EqualsApprox(Quaternion(0,  0.148035f,  0.905278f, -0.398192f)));
	CHECK_THAT(Quaternion::Slerp(q1, q2, 0.4f), EqualsApprox(Quaternion(0, -0.0245656f, 0.863327f, -0.504046f)));
	CHECK_THAT(Quaternion::Slerp(q1, q2, 0.5f), EqualsApprox(Quaternion(0, -0.196116f,  0.784465f, -0.588348f)));
	CHECK_THAT(Quaternion::Slerp(q1, q2, 0.6f), EqualsApprox(Quaternion(0, -0.359282f,  0.672061f, -0.647496f)));
	CHECK_THAT(Quaternion::Slerp(q1, q2, 0.7f), EqualsApprox(Quaternion(0, -0.507086f,  0.530923f, -0.678959f)));
	CHECK_THAT(Quaternion::Slerp(q1, q2, 0.8f), EqualsApprox(Quaternion(0, -0.633209f,  0.367085f, -0.681392f)));
	CHECK_THAT(Quaternion::Slerp(q1, q2, 0.9f), EqualsApprox(Quaternion(0, -0.732259f,  0.187552f, -0.654692f)));
	CHECK_THAT(Quaternion::Slerp(q1, q2, 1), EqualsApprox(Quaternion(0, -0.8f, 0, -0.6f)));

	CHECK_THAT(Quaternion::Slerp(q1, q1, 0.1f), EqualsApprox(q1));
	CHECK_THAT(Quaternion::Slerp(q1, q1, 0.5f), EqualsApprox(q1));
}
