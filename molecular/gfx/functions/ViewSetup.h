/*	ViewSetup.h
	Copyright 2016 Fabian Herb

	This file is part of Molecular Engine.
*/

#ifndef VIEWSETUP_H
#define VIEWSETUP_H

#include <molecular/gfx/RenderFunction.h>
#include <molecular/util/Matrix4.h>


namespace molecular
{
class Quaternion;
class RenderContext;

namespace Gfx
{

class ViewSetup : public SingleCalleeRenderFunction
{
public:
	template<class TRenderManager>
	ViewSetup(TRenderManager& manager, RenderContext& renderContext) :
		SingleCalleeRenderFunction(manager),
		mRenderContext(renderContext)
	{}

	template<class TRenderManager>
	ViewSetup(TRenderManager& manager) :
		SingleCalleeRenderFunction(manager),
		mRenderContext(manager.GetRenderContext())
	{}


	void Execute() override;

	/// Set projection matrix to a perspective projection
	/** @todo Don't pass aspect ratio here. Recalculate on viewport change. */
	void SetProjectionPerspective(float fieldOfView, float nearPlane, float farPlane);

	/// Set projection matrix to an orthographic projection
	void SetProjectionOrthographic(float width, float height, float nearPlane, float farPlane);

	/// Sets the view matrix according to a camera with the given parameters
	/** This is cheaper than calling SetView with an inverted matrix. */
	void SetCamera(const Vector3& position, const Quaternion& orientation);

	/// Set view matrix
	void SetCamera(const Matrix4& cameraMatrix) {mCamera = cameraMatrix;}

private:
	Matrix4 CalculateProjectionMatrix(float viewportAspectRatio);

	RenderContext& mRenderContext;
	bool mOrthographic = false;
	float mNear = 0.1f, mFar = 100.0f;
	float mFieldOfView = 1.0f;
	float mOrthographicWidth = 10.0f;
	float mOrthographicHeight = 10.0f;

	Matrix4 mCamera;
};

}
}

#endif // VIEWSETUP_H
