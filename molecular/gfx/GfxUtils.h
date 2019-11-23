/*	GfxUtils.h

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

#ifndef MOLECULAR_GFXUTILS_H
#define MOLECULAR_GFXUTILS_H

#include <molecular/util/Vector3.h>
#include <molecular/util/Matrix4.h>

namespace molecular
{
namespace gfx
{
using namespace util;

/// Convert screen coordinates to world coordinates
/** Similar to gluUnProject().
	@param proj Projection matrix.
	@param view View matrix.
	@param point Point to unproject in normalised device coordinates (from -1 to 1 in all axes).
	@returns Point in world coordinates. */
Vector3 Unproject(const Matrix4& proj, const Matrix4& view, const Vector3& point)
{
	Matrix4 unproj = (proj * view).Inverse();
	Vector4 worldCoords = unproj * Vector4(point, 1.0f);
	float div = 1.0f / worldCoords[3];
	return Vector3(worldCoords[0] * div, worldCoords[1] * div, worldCoords[2] * div);
}

}
}

#endif // GFXUTILS_H

