/*	PlaneSet.h

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

#ifndef MOLECULAR_PLANESET_H
#define MOLECULAR_PLANESET_H

#include <molecular/util/Vector3.h>
#include "Plane.h"
#include <array>

namespace molecular
{
namespace util
{
namespace PlaneSet
{

/// Clip polygon by set of planes
/** @see Plane::ClipPolygon() */
template<class PlaneIterator, class PointInputIterator, class PointOutputIterator>
PointOutputIterator ClipPolygon(
        PlaneIterator planesBegin, PlaneIterator planesEnd,
        PointInputIterator pointsInputBegin, PointInputIterator pointInputEnd,
        PointOutputIterator pointsOutputBegin)
{
	assert(planesBegin != planesEnd);

	// We use two buffers as interchanging roles as source and target.
	std::array<Vector3, 128> tempPolygon1;
	std::array<Vector3, 128> tempPolygon2;

	auto sourcePolygonBegin = tempPolygon1.begin();
	auto sourcePolygonEnd = tempPolygon1.end();
	auto destPolygonBegin = tempPolygon2.begin();
	auto destPolygonEnd = tempPolygon2.end();

	for(auto nplane = planesBegin; nplane != planesEnd; ++ nplane )
	{
		std::swap(sourcePolygonBegin, destPolygonBegin);
		std::swap(sourcePolygonEnd, destPolygonEnd);

		if(nplane == planesBegin)
			destPolygonEnd = nplane->ClipPolygon(pointsInputBegin, pointInputEnd, destPolygonBegin);
		else
			destPolygonEnd = nplane->ClipPolygon(sourcePolygonBegin, sourcePolygonEnd, destPolygonBegin);

		// If the polygon was completely on the backside of the plane,
		// then polygon is outside the frustum.
		if(destPolygonBegin == destPolygonEnd)
			return pointsOutputBegin;
	}

	return std::copy(destPolygonBegin, destPolygonEnd, pointsOutputBegin);
}

/// Generate culling planes from polygon
/** For perspective projections. Does not generate near and far plane. */
template<class PointIterator, class PlaneOutputIterator>
PlaneOutputIterator CreateCullingPlanes(const Vector3& viewPos, PointIterator polygonBegin, PointIterator polygonEnd, PlaneOutputIterator planesBegin)
{
	auto outputIt = planesBegin;
	for(auto it = polygonBegin; it != polygonEnd; ++it)
	{
		auto v2It = it;
		IncrementAndWrap(v2It, polygonBegin, polygonEnd);
		*outputIt++ = Plane(viewPos, *v2It, *it); // CCW
	}
	return outputIt;
}

/// Check intersection of object
/** @see Plane::Check
	@see Frustum::Check */
template<class PlaneIterator, class Object>
Plane::IntersectStatus CheckIntersection(PlaneIterator planesBegin, PlaneIterator planesEnd, Object object)
{
	bool intersecting = false;

	for(auto plane = planesBegin; plane != planesEnd; ++plane)
	{
		Plane::IntersectStatus status = plane->Check(object);
		if(status == Plane::kOutside)	// outside of plane
			return Plane::kOutside;
		if(status == Plane::kIntersect)
			intersecting = true;
	}
	if(intersecting)
		return Plane::kIntersect;
	else
		return Plane::kInside;	// inside of all planes
}

}
}
}

#endif // MOLECULAR_PLANESET_H
