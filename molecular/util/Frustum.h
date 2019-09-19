/*	Frustum.cpp
	Copyright 2012-2017 Fabian Herb

	This file is part of Molecular Engine.
*/

#ifndef Frustum_h
#define Frustum_h

#include "Plane.h"
#include <molecular/util/Matrix.h>
#include <array>

namespace molecular
{
namespace util
{

class AxisAlignedBox;

/// A frustum contains the space that is seen
class Frustum
{
public:
	Frustum(const Matrix<4, 4>& viewProjectionMatrix);
	Frustum(const Plane planes[6]);
	
	Plane::IntersectStatus Check(const Vector3& point) const;
	Plane::IntersectStatus Check(const Vector3&	pos, float radius) const;
	Plane::IntersectStatus Check(const AxisAlignedBox& box) const;
	
	enum PlaneNum
	{
		kNear = 0,
		kFar = 1,
		kLeft = 2,
		kRight = 3,
		kTop = 4,
		kBottom = 5
	};

	const Plane& GetPlane(PlaneNum plane) const { return mPlanes[plane];}
	const std::array<Plane, 6>& GetPlanes() const {return mPlanes;}

private:
	/// Enclosing planes
	/** Normals pointing inwards. */
	std::array<Plane, 6> mPlanes;
};

}
}

std::ostream& operator<<(std::ostream& o, const molecular::util::Frustum& f);


#endif
