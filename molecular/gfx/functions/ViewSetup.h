/*	ViewSetup.h

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

#ifndef MOLECULAR_VIEWSETUP_H
#define MOLECULAR_VIEWSETUP_H

#include <molecular/gfx/RenderFunction.h>
#include <molecular/util/Matrix4.h>


namespace molecular
{
class Quaternion;
class RenderContext;

namespace gfx
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

#endif // MOLECULAR_VIEWSETUP_H
