/*	Box.h
	Copyright 2012-2015 Fabian Herb

	This file is part of Molecular Engine.
*/
#ifndef BOX_H
#define BOX_H

#include <molecular/util/Vector4.h>
#include <molecular/util/Matrix4.h>

namespace molecular
{
namespace util
{

class AxisAlignedBox;

/// Represents an arbitrarily oriented box in 3D space
/** @see AxisAlignedBox */
class Box
{
public:
	Box(const Vector3& sizes = Vector3(1,1,1), const Vector3& position = Vector3(0,0,0));
	Box(const AxisAlignedBox& aaBox);

	/// Generates AABB that contains this box
	AxisAlignedBox ToAxisAligned() const;
	Vector3 GetCorner(bool xMax, bool yMax, bool zMax) const;

	void Transform(const Matrix4& trans);

private:
	/// Transforms a unit box to the actual box
	/** The unit box has the bounds -0.5 to 0.5 on every axis and sits at
		(0,0,0).*/
	Matrix4 mTransform;

	/// Corners of the unit box
	static const Vector4 kCorners[8];
};

}
}

#endif // BOX_H
