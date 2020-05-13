/*	ViewSetup.cpp

MIT License

Copyright (c) 2019-2020 Fabian Herb

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

#include "ViewSetup.h"
#include <molecular/gfx/RenderContext.h>
#include <molecular/util/Quaternion.h>
#include <molecular/util/Matrix3.h>
#include <molecular/util/Math.h>

namespace molecular
{
namespace gfx
{

void ViewSetup::HandleExecute(Scope& scope)
{
	if(!mCallee)
		return;

	scope.Set("gl_Position"_H, Output());
	scope.Set("fragmentColor"_H, Output());

	for(int eye = 0; eye < mRenderContext.GetNumEyes(); ++eye)
	{
		auto viewport = mRenderContext.GetViewport(eye);
		auto renderTarget = mRenderContext.GetRenderTarget(eye);
		scope.Set("viewportSite"_H, Uniform<Vector2>(Vector2(viewport[2], viewport[3])));

		const Matrix4 viewMatrix = (mCamera * mRenderContext.GetHeadToEyeTransform(eye)).Inverse();
		scope.Set("viewMatrix"_H, Uniform<Matrix4>(viewMatrix));

		mRenderer.SetBaseTarget(viewport, renderTarget);
		mRenderer.Clear(true, true);

		Matrix4 projectionMatrix;
		if(mRenderContext.HasProjectionMatrix(eye))
		{
			// Set projection matrix from RenderContext
			projectionMatrix = mRenderContext.GetProjectionMatrix(eye);
		}
		else
		{
			float viewportAspectRatio = float(viewport[2]) / viewport[3];
			projectionMatrix = CalculateProjectionMatrix(viewportAspectRatio);
		}
		scope.Set("projectionMatrix"_H, Uniform<Matrix4>(projectionMatrix));
		mCallee->Execute(scope);
	}

}

void ViewSetup::SetProjectionPerspective(float fieldOfView, float nearPlane, float farPlane)
{
	mOrthographic = false;
	mFieldOfView = fieldOfView;
	mNear = nearPlane;
	mFar = farPlane;
}

void ViewSetup::SetProjectionOrthographic(float width, float height, float nearPlane, float farPlane)
{
	mOrthographic = true;
	mOrthographicWidth = width;
	mOrthographicHeight = height;
	mNear = nearPlane;
	mFar = farPlane;
}

void ViewSetup::SetCamera(const Vector3& position, const Quaternion& orientation)
{
	mCamera = Matrix4::Translation(position[0], position[1], position[2]) * Matrix4(Matrix3(orientation));
}


Matrix4 ViewSetup::CalculateProjectionMatrix(float viewportAspectRatio)
{
	if(mOrthographic)
	{
		float requestedAspectRatio = mOrthographicWidth / mOrthographicHeight;
		float width, height;
		if(viewportAspectRatio > requestedAspectRatio)
		{
			height = mOrthographicHeight;
			width = height * viewportAspectRatio;
		}
		else
		{
			width = mOrthographicWidth;
			height = width / viewportAspectRatio;
		}
		return Matrix4::ProjectionOrthographic(width, height, mNear, mFar);
	}
	else
	{
		return Matrix4::ProjectionPerspective(mFieldOfView, viewportAspectRatio, mNear, mFar);
	}
}

}
}
