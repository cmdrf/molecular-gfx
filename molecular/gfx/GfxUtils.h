/*	GfxUtils.h
	Copyright 2015 Fabian Herb

	This file is part of Molecular Engine.
*/

#ifndef GFXUTILS_H
#define GFXUTILS_H

#include "util/Vector3.h"
#include "util/Matrix4.h"

namespace gfx
{
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

#endif // GFXUTILS_H

