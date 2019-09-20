/*	Box.h

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

#ifndef MOLECULAR_BOX_H
#define MOLECULAR_BOX_H

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

#endif // MOLECULAR_BOX_H
