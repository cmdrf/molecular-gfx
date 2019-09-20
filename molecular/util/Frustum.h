/*	Frustum.cpp

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
SOFTWARE.*/

#ifndef MOLECULAR_FRUSTUM_H
#define MOLECULAR_FRUSTUM_H

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
