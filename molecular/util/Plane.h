/*	Plane.h

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

#ifndef MOLECULAR_UTIL_PLANE_H
#define MOLECULAR_UTIL_PLANE_H

#include <molecular/util/Vector3.h>
#include <molecular/util/AxisAlignedBox.h>
#include <array>
#include <sstream>

namespace molecular
{
namespace util
{

/// Plane in Hesse normal form
class Plane
{
public:
	/// Default plane. Spans XY plane, points towards +Z
	Plane() :
		mNormal(0, 0, 1),
		mDistance(0)
	{}

	/// Construct from normal and distance, normal as Vector3
	Plane(const Vector3& normal, float distance) :
		mNormal(normal),
		mDistance(distance)
	{}

	/// Construct from normal and distance, normal as individual components
	Plane(float n0, float n1, float n2, float d) :
		mNormal(n0, n1, n2),
		mDistance(d)
	{}

	/// Construct plane passing through three points
	/** Normal is positive for the side on which the points are defined CCW. */
	inline Plane(const Vector3& p0, const Vector3& p1, const Vector3& p2);

	/// Return type of Check()
	enum IntersectStatus
	{
		kOutside,
		kInside,
		kIntersect
	};

	/// Get distance of point to plane
	float GetDistance(const Vector3& point) const
	{
		return mNormal.DotProduct(point) - mDistance;
	}

	/// Check point
	/** Never returns kIntersect, obviously. */
	IntersectStatus Check(const Vector3& point) const
	{
		float distance = GetDistance(point);
		if(distance > 0.001f) // ClipPolygon needs to find points that are on the plane
			return kInside;
		if(distance < -0.001f)
			return kOutside;
		return kIntersect;
	}

	/// Check sphere
	IntersectStatus Check(const Vector3& center, float radius) const
	{
		float distance = GetDistance(center);
		if(distance < -radius)
			return kOutside;
		if(std::abs(distance) < radius)
			return kIntersect;
		return kInside;
	}

	/// Check AABB
	IntersectStatus Check(const AxisAlignedBox& box) const
	{
		Vector3 center = box.GetCenter();
		Vector3 halfSizes = box.GetSize() * 0.5;
		float distance = GetDistance(center);
		float absDot = std::abs(mNormal[0] * halfSizes[0]) + std::abs(mNormal[1] * halfSizes[1]) + std::abs(mNormal[2] * halfSizes[2]);

		if (distance < -absDot)
			return kOutside;

		if (distance > +absDot)
			return kInside;

		return kIntersect;
	}

	/// Scale normal and vector so that normal has unit length
	/** Values describes the same plane before and after normalization. */
	void Normalize()
	{
		float invLength = 1.0f / mNormal.Length();
		mNormal *= invLength;
		mDistance *= invLength;
	}

	const Vector3& GetNormal() const {return mNormal;}
	float GetDistance() const {return mDistance;}
	void SetDistance(float distance) {mDistance = distance;}

	/// Clip polygon by plane
	/** Output range must have space for one more point than input range.
		@return Iterator to end of new output range. Equal to destBegin if entire polygon lies on
			the negative side of the plane, e.g. no polygon remains after clipping. */
	template<class InputIterator, class OutputIterator>
	OutputIterator ClipPolygon(InputIterator inputBegin, InputIterator inputEnd, OutputIterator destBegin);

	/// Get point where line intersects with plane
	/** Line is given by two endpoints.
		@param p0 One endpoint of the line.
		@param p1 The other endpoint of the line. */
	inline Vector3 IntersectionPoint(const Vector3& p0, const Vector3& p1);

private:
	Vector3 mNormal;
	float mDistance;
};

inline std::ostream& operator<<(std::ostream& o, const Plane& p)
{
	o << '(' << p.GetNormal() << ", " << p.GetDistance() << ')';
	return o;
}

inline std::ostream& operator<<(std::ostream& o, const Plane::IntersectStatus& i)
{
	switch(i)
	{
	case Plane::kOutside:
		o << "kOutside";
		break;

	case Plane::kInside:
		o << "kInside";
		break;

	case Plane::kIntersect:
		o << "kIntersect";
		break;
	};
	return o;
}

inline Plane::Plane(const Vector3& p0, const Vector3& p1, const Vector3& p2)
{
	Vector3 kj = p0 - p1;
	Vector3 lj = p0 - p2;
	mNormal = kj.CrossProduct(lj).Normalized();
	mDistance = p1.DotProduct(mNormal);
}

template<class Iterator>
void IncrementAndWrap(Iterator& it, Iterator begin, Iterator end)
{
	++it;
	if(it == end)
		it = begin;
}

template<class Iterator>
void DecrementAndWrap(Iterator& it, Iterator begin, Iterator end)
{
	if(it == begin)
		it = end - 1; // Wrap around
	else
		--it;
}

template<class InputIterator, class OutputIterator>
OutputIterator Plane::ClipPolygon(InputIterator inputBegin, InputIterator inputEnd, OutputIterator outBegin)
{
	assert(inputEnd - inputBegin <= 128);
	std::array<Vector3, 128> intermediatePoints;
	auto interEnd = std::copy(inputBegin, inputEnd, intermediatePoints.begin());
	auto interBegin = intermediatePoints.begin();

	// Find the first vertex that lies on the front of the plane.
	auto start = interEnd;
	for(auto i = interBegin; i != interEnd; i++ )
	{
		if(Check(*i) == kInside)
		{
			start = i;
			break;
		}
	}

	// If nothing was in front of the plane, we're done.
	if(start == interEnd)
		return outBegin;

	auto baseStart = start;
	auto end = start;
	IncrementAndWrap(end, interBegin, interEnd);

	auto outIterator = outBegin;
	while(end != baseStart)
	{
		IntersectStatus startSide = Check(*start);
		IntersectStatus endSide = Check(*end);

		*outIterator++ = *start;
		if(startSide != kOutside && endSide != kOutside)
		{
			// No clipping required
			start = end;
			IncrementAndWrap(end, interBegin, interEnd);
		}
		else if(startSide != kOutside && endSide == kOutside)
		{
			if(startSide == kInside) // Insert the intersection:
				*outIterator++ = IntersectionPoint(*start, *end);

			// Advance to first point that is in front or on, use as next end point:
			do
			{
				IncrementAndWrap(end, interBegin, interEnd);
			}while(Check(*end) == kOutside);

			// Step back by one to find start point:
			start = end;
			DecrementAndWrap(start, interBegin, interEnd);

			*start = IntersectionPoint(*start, *end);
		}
		else
		{
			std::ostringstream oss;
			oss << "Bad combination: " << startSide << ", " << endSide << "; ";
			oss << "Plane: " << *this << "; Polygon: ";
			for(auto it = inputBegin; it != inputEnd; ++it)
				oss << *it << ", ";
			throw std::runtime_error(oss.str());
		}
	}

	// Emit the last point.
	*outIterator++ = *start;

	return outIterator;
}

inline Vector3 Plane::IntersectionPoint(const Vector3& p0, const Vector3& p1)
{
	Vector3 vector = p1 - p0;
	float t = -GetDistance(p0) / mNormal.DotProduct(vector);

	return p0 + (vector * t);
}

}
}

#endif // MOLECULAR_PLANE_H
