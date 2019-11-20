/*	TextureUtils.h

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

#ifndef MOLECULAR_TEXTUREUTILS_H
#define MOLECULAR_TEXTUREUTILS_H

#include "RenderCmdSink.h"
#include <molecular/util/StringUtils.h>

namespace molecular
{
namespace gfx
{
namespace TextureUtils
{
RenderCmdSink::Texture* Random1DTexture(RenderCmdSink& renderer, unsigned int extent);

template<class TFileServer>
RenderCmdSink::Texture* Fbm3DTexture(RenderCmdSink& renderer, TFileServer& fileServer, unsigned int extent);

RenderCmdSink::Texture* SpotTexture(RenderCmdSink& renderer, unsigned int width, unsigned int height);

/*****************************************************************************/

template<class TFileServer>
RenderCmdSink::Texture* Fbm3DTexture(RenderCmdSink& renderer, TFileServer& fileServer, unsigned int extent)
{
	auto fbmVertFile = fileServer.Open("nvparticles/fbm.vert");
	auto fbmFragFile = fileServer.Open("nvparticles/fbm.frag");
	RenderCmdSink::Program* fbmProgram = renderer.CreateProgram();
	auto fbmVertSrc = StringUtils::FromStorage(fbmVertFile);
	auto fbmFragSrc = StringUtils::FromStorage(fbmFragFile);
	fbmProgram->Store(fbmVertSrc, fbmFragSrc);

	RenderCmdSink::Texture* out = renderer.CreateTexture();
	out->Store(extent, extent, extent, NULL, PF_RGBA_FLOAT16);
	out->SetParameter(RenderCmdSink::Texture::kMinFilter, RenderCmdSink::Texture::kLinear);
	out->SetParameter(RenderCmdSink::Texture::kMagFilter, RenderCmdSink::Texture::kLinear);

	RenderCmdSink::RenderTarget* framebuffer = renderer.CreateRenderTarget();
	RenderCmdSink::VertexBuffer* quadVertexBuffer = renderer.CreateVertexBuffer();

	const float position[] = {
		-1.0f, -1.0f,
		 1.0f, -1.0f,
		-1.0f,  1.0f,
		 1.0f,  1.0f,
	};

	quadVertexBuffer->Store(position, sizeof(float) * 8);
	fbmProgram->SetAttribute("attr_position"_H, quadVertexBuffer, 2, VertexAttributeInfo::kFloat);

	for(unsigned int n = 0; n < extent; n++)
	{
		const float layer = n/(float)extent;

		framebuffer->AttachColorBuffer(out, 0, n);
		renderer.SetTarget(framebuffer);

		renderer.UseProgram(fbmProgram);
		fbmProgram->SetUniform("u_Layer"_H, &layer);

		renderer.Draw(IndexBufferInfo::Mode::kTriangleStrip, 4);
	}
	renderer.SetTarget(nullptr);
	renderer.DestroyVertexBuffer(quadVertexBuffer);
	renderer.DestroyRenderTarget(framebuffer);
	renderer.DestroyProgram(fbmProgram);

	return out;
}

} }
} // molecular

#endif // TEXTUREUTILS_H
