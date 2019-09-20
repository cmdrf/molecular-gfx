/*	RenderContext.h
	Copyright 2016 Fabian Herb

	This file is part of Molecular Engine.
*/

#ifndef RENDERCONTEXT_H
#define RENDERCONTEXT_H

#include <molecular/util/Vector.h>
#include <molecular/util/Matrix4.h>
#include <cassert>

namespace molecular
{

/// Provides a context for the Renderer to draw to
class RenderContext
{
public:
	virtual ~RenderContext() {}

	virtual int GetNumEyes() {return 1;}
	virtual IntVector4 GetViewport(int eye) = 0;
	virtual intptr_t GetRenderTarget(int eye) {assert(eye == 0); return 0;}
	virtual Matrix4 GetHeadToEyeTransform(int eye) {assert(eye == 0); return Matrix4::Identity();}
	virtual bool HasProjectionMatrix(int eye) {assert(eye == 0); return false;}
	virtual Matrix4 GetProjectionMatrix(int eye) {assert(false); return Matrix4::Identity();}
};

}

#endif // RENDERCONTEXT_H
