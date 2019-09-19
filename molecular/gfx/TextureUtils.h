/*	TextureUtils.h
	Copyright 2015-2016 Fabian Herb

	This file is part of Molecular Engine.
*/

#ifndef TEXTUREUTILS_H
#define TEXTUREUTILS_H

#include "RenderCmdSink.h"
#include <molecular/util/StringUtils.h>

namespace molecular
{
namespace Gfx
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

	for(int n = 0; n < extent; n++)
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
