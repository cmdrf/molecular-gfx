/*	RenderContext.h

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

#ifndef MOLECULAR_RENDERCONTEXT_H
#define MOLECULAR_RENDERCONTEXT_H

#include <molecular/util/Vector.h>
#include <molecular/util/Matrix4.h>
#include <cassert>

namespace molecular
{
namespace gfx
{

/// Provides a context for the Renderer to draw to
class RenderContext
{
public:
	virtual ~RenderContext() = default;

	virtual int GetNumEyes() {return 1;}
	virtual IntVector4 GetViewport(int eye) = 0;
	virtual intptr_t GetRenderTarget(int eye) {assert(eye == 0); return 0;}
	virtual Matrix4 GetHeadToEyeTransform(int eye) {assert(eye == 0); return Matrix4::Identity();}
	virtual bool HasProjectionMatrix(int eye) {assert(eye == 0); return false;}
	virtual Matrix4 GetProjectionMatrix(int /*eye*/) {assert(false); return Matrix4::Identity();}
};

}
}

#endif // MOLECULAR_RENDERCONTEXT_H
