/*	Box.cpp
	Copyright 2012-2017 Fabian Herb

	This file is part of Molecular Engine.
*/

#include "Box.h"
#include "Vector4.h"
#include "AxisAlignedBox.h"

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
