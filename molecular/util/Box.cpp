/*	Box.cpp

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

#include "Box.h"
#include <molecular/util/Vector4.h>
#include <molecular/util/AxisAlignedBox.h>

namespace molecular
{
namespace util
{

const Vector4 Box::kCorners[8] = {
	Vector4(-0.5, -0.5, -0.5),
	Vector4(-0.5, -0.5,  0.5),
	Vector4(-0.5,  0.5, -0.5),
	Vector4(-0.5,  0.5,  0.5),
	Vector4( 0.5, -0.5, -0.5),
	Vector4( 0.5, -0.5,  0.5),
	Vector4( 0.5,  0.5, -0.5),
	Vector4( 0.5,  0.5,  0.5)
};

Box::Box(const Vector3& sizes, const Vector3& center) :
	mTransform(Matrix4::Translation(center[0], center[1], center[2]) * Matrix4::Scale(sizes[0], sizes[1], sizes[2]))
{
}

Box::Box(const AxisAlignedBox& aaBox)
{
	Vector3 sizes = aaBox.GetMax() - aaBox.GetMin();
	Vector3 center = aaBox.GetCenter();
	mTransform = Matrix4::Translation(center[0], center[1], center[2]) * Matrix4::Scale(sizes[0], sizes[1], sizes[2]);
}

AxisAlignedBox Box::ToAxisAligned() const
{
	AxisAlignedBox box;
	for(auto& corner: kCorners)
	{
		box.Stretch((mTransform * corner).Xyz());
	}
	return box;
}

Vector3 Box::GetCorner(bool xMax, bool yMax, bool zMax) const
{
	int corner = 0;
	if(xMax)
		corner = 4;
	if(yMax)
		corner |= 2;
	if(zMax)
		corner |= 1;
	return (mTransform * kCorners[corner]).Xyz();
}

void Box::Transform(const Matrix4& trans)
{
	mTransform = trans * mTransform;
}

}
}
