/*	GlCommandSink.cpp

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

#include "GlCommandSink.h"
#include "GlCommandSinkProgram.h"
#include "PixelFormatConversion.h"
#include <cassert>
#include <molecular/util/Logging.h>

namespace molecular
{
GlFunctions GlCommandSink::gl;

void GlCommandSink::Init()
{
	gl.Init();

	LOG(INFO) << "GL_VERSION: " << gl.GetString(gl.VERSION);
	LOG(INFO) << "GL_SHADING_LANGUAGE_VERSION: " << gl.GetString(gl.SHADING_LANGUAGE_VERSION);
	LOG(INFO) << "GL_EXTENSIONS: ";
	GLint numExtensions = 0;
	gl.GetIntegerv(gl.NUM_EXTENSIONS, &numExtensions);
	for(GLint i = 0; i < numExtensions; ++i)
		LOG(INFO) << "    " << gl.GetStringi(gl.EXTENSIONS, i);

	GLint numCompressedTextureFormats = 0;
	gl.GetIntegerv(gl.NUM_COMPRESSED_TEXTURE_FORMATS, &numCompressedTextureFormats);
	if(GLenum error = glGetError())
		LOG(ERROR) << "glGetIntegerv(GL_COMPRESSED_TEXTURE_FORMATS): " << GlConstantString(error);
	else
	{
		LOG(INFO) << "GL_COMPRESSED_TEXTURE_FORMATS[" << numCompressedTextureFormats << "]: ";
		std::vector<GLint> compressedTextureFormats(numCompressedTextureFormats);
		gl.GetIntegerv(gl.COMPRESSED_TEXTURE_FORMATS, compressedTextureFormats.data());
		for(GLint i = 0; i < numCompressedTextureFormats; ++i)
			LOG(INFO) << "    " << GlConstantString(compressedTextureFormats[i]);
	}
	gl.Enable(gl.FRAMEBUFFER_SRGB);
	CheckError("glEnable", __LINE__);

	GLint colorEncoding = 0;
	gl.GetFramebufferAttachmentParameteriv(gl.FRAMEBUFFER, gl.FRONT_LEFT, gl.FRAMEBUFFER_ATTACHMENT_COLOR_ENCODING, &colorEncoding);
	CheckError("glGetFramebufferAttachmentParameteriv", __LINE__);
	LOG(INFO) << "GL_FRAMEBUFFER_ATTACHMENT_COLOR_ENCODING: " << GlConstantString(colorEncoding);

	gl.Enable(gl.DEPTH_TEST);
	gl.Enable(gl.CULL_FACE);
	SetTarget(nullptr);

	if(gl.HasPrimitiveRestartIndex())
	{
		gl.PrimitiveRestartIndex(0xffff);
		gl.Enable(gl.PRIMITIVE_RESTART);
	}

#ifndef OPENGL_ES3
	gl.Enable(gl.PROGRAM_POINT_SIZE); // Always enabled on GLES3
#endif
}

void GlCommandSink::VertexBuffer::Store(const void* data, size_t size, bool stream)
{
	gl.BindBuffer(gl.ARRAY_BUFFER, mBuffer);
	CheckError("glBindBuffer", __LINE__);
	if(stream)
	{
		// https://www.opengl.org/wiki/Buffer_Object_Streaming#Buffer_re-specification
		gl.BufferData(gl.ARRAY_BUFFER, size, nullptr, gl.STREAM_DRAW);
	}
	gl.BufferData(gl.ARRAY_BUFFER, size, data, stream ? gl.STREAM_DRAW : gl.STATIC_DRAW);
	CheckError("glBufferData", __LINE__);
}

void GlCommandSink::IndexBuffer::Store(const void* data, size_t size)
{
	gl.BindBuffer(gl.ELEMENT_ARRAY_BUFFER, mBuffer);
	CheckError("glBindBuffer", __LINE__);
	gl.BufferData(gl.ELEMENT_ARRAY_BUFFER, size, data, gl.STATIC_DRAW);
	CheckError("glBufferData", __LINE__);
}

GlCommandSink::Texture* GlCommandSink::CreateTexture() {return new Texture;}

void GlCommandSink::DestroyTexture(Texture* texture) {delete texture;}

GlCommandSink::Program* GlCommandSink::CreateProgram()
{
	return new Program;
}

void GlCommandSink::DestroyProgram(Program* program)
{
	delete program;
}

void GlCommandSink::UseProgram(Program* program)
{
	if(program)
	{
#ifndef NDEBUG
		gl.ValidateProgram(program->mProgram);
		CheckError("glValidateProgram", __LINE__);
		GLint status;
		gl.GetProgramiv(program->mProgram, gl.VALIDATE_STATUS, &status);
		CheckError("glGetProgramiv", __LINE__);
		if(status != GL_TRUE)
			LOG(ERROR) << "GL_VALIDATE_STATUS == GL_FALSE";
#endif

		gl.UseProgram(program->mProgram);
		CheckError("glUseProgram", __LINE__);
		gl.BindVertexArray(program->mVertexArrayObject);
		CheckError("glBindVertexArray", __LINE__);
	}
	else
	{
		gl.UseProgram(0);
		CheckError("glUseProgram", __LINE__);
		gl.BindVertexArray(0);
		CheckError("glBindVertexArray", __LINE__);
	}
}

GlCommandSink::RenderTarget* GlCommandSink::CreateRenderTarget()
{
	return new RenderTarget;
}

void GlCommandSink::DestroyRenderTarget(RenderTarget* target)
{
	delete target;
}

void GlCommandSink::SetTarget(RenderTarget* target)
{
//	if(target == mCurrentTarget)
//		return;
	if(target)
	{
		gl.BindFramebuffer(gl.FRAMEBUFFER, target->mFramebuffer);
		CheckError("glBindFramebuffer", __LINE__);
		if(target->ColorBufferAttached())
		{
			GLenum drawBuffer = gl.COLOR_ATTACHMENT0;
			gl.DrawBuffers(1, &drawBuffer);
			CheckError("glDrawBuffers", __LINE__);
			gl.ReadBuffer(gl.COLOR_ATTACHMENT0);
			CheckError("glReadBuffer", __LINE__);
		}
		else
		{
			GLenum drawBuffer = GL_NONE;
			gl.DrawBuffers(1, &drawBuffer);
			CheckError("glDrawBuffers", __LINE__);
			gl.ReadBuffer(GL_NONE);
			CheckError("glReadBuffer", __LINE__);
		}
#ifndef NDEBUG
		GLenum status = gl.CheckFramebufferStatus(gl.FRAMEBUFFER);
		CheckError("glCheckFramebufferStatus", __LINE__);
		if(status != gl.FRAMEBUFFER_COMPLETE)
		{
			LOG(ERROR) << std::hex << "glCheckFramebufferStatus Return: " << status;
			return;
		}
#endif
		gl.Viewport(0, 0, target->GetWidth(), target->GetHeight());
		CheckError("glViewport", __LINE__);
	}
	else
	{
		gl.BindFramebuffer(gl.FRAMEBUFFER, mBaseTargetFramebuffer);
		CheckError("glBindFramebuffer", __LINE__);
#ifdef OPENGL_ES3
//		GLenum drawBuffer = GL_BACK;
#else
//		GLenum drawBuffer = GL_BACK_LEFT;
#endif
//		gl.DrawBuffers(1, &drawBuffer);
//		CheckError("glDrawBuffers", __LINE__);
//		gl.ReadBuffer(drawBuffer);
//		CheckError("glReadBuffer", __LINE__);
		gl.Viewport(mBaseTargetViewport[0], mBaseTargetViewport[1], mBaseTargetViewport[2], mBaseTargetViewport[3]);
		CheckError("glViewport", __LINE__);
	}
	mCurrentTarget = target;
}

void GlCommandSink::SetBaseTarget(const IntVector4& viewport, int renderTarget)
{
	mBaseTargetViewport = viewport;
	mBaseTargetFramebuffer = renderTarget;
}

GlCommandSink::TransformFeedback* GlCommandSink::CreateTransformFeedback()
{
	GLuint feedback;
	gl.GenTransformFeedbacks(1, &feedback);
	return new TransformFeedback(feedback);
}

void GlCommandSink::DestroyTransformFeedback(TransformFeedback* feedback)
{
	delete feedback;
}

void GlCommandSink::Clear(bool color, bool depth)
{
	GLbitfield bits = 0;
	if(color)
		bits |= GL_COLOR_BUFFER_BIT;
	if(depth)
		bits |= GL_DEPTH_BUFFER_BIT;
	gl.Clear(bits);
}

void GlCommandSink::ReadPixels(int x, int y, int width, int height, PixelFormat format, void* data)
{
	gl.ReadPixels(x, y, width, height, PixelFormatConversion::ToGlFormat(format), PixelFormatConversion::ToGlType(format), data);
	CheckError("glReadPixels", __LINE__);
}

void GlCommandSink::SetBlending(bool enable, BlendFactor sourceFactor, BlendFactor destFactor)
{
	if(enable)
	{
		glEnable(GL_BLEND);
		glBlendFunc(sourceFactor, destFactor);
	}
	else
		glDisable(GL_BLEND);
}

void GlCommandSink::SetDepthState(bool depthTest, bool depthWrite, CompareOp compareOp)
{
	if(depthTest)
		glEnable(gl.DEPTH_TEST);
	else
		glDisable(gl.DEPTH_TEST);
	glDepthMask(depthWrite ? GL_TRUE : GL_FALSE);
	glDepthFunc(compareOp);
}


void GlCommandSink::SetScissor(bool enable, int x, int y, int width, int height)
{
    if(enable)
    {
        glEnable(GL_SCISSOR_TEST);
        glScissor(x, y, width, height);
    }
    else
        glDisable(GL_SCISSOR_TEST);
}

void GlCommandSink::SetRasterizationState(bool rasterizerDiscard, CullMode cullMode)
{
	if(rasterizerDiscard)
		glEnable(gl.RASTERIZER_DISCARD);
	else
		glDisable(gl.RASTERIZER_DISCARD);
	glCullFace(cullMode);
}

void GlCommandSink::Draw(IndexBuffer* buffer, const IndexBufferInfo& info)
{
	assert(buffer);
	gl.BindBuffer(gl.ELEMENT_ARRAY_BUFFER, buffer->mBuffer);
	CheckError("glBindBuffer", __LINE__);
	gl.DrawElements(ToGlEnum(info.mode), info.count, ToGlEnum(info.type), reinterpret_cast<const GLvoid*>(info.offset));
	CheckError("glDrawElements", __LINE__);

/*	Found on the web:
	GL_INVALID_OPERATION is generated if a non-zero buffer object name is bound
		to an enabled array or to the GL_DRAW_INDIRECT_BUFFER binding and the
		buffer object's data store is currently mapped.
	GL_INVALID_OPERATION is generated if glDrawArrays is executed between the
		execution of glBegin and the corresponding glEnd.
	GL_INVALID_OPERATION will be generated by glDrawArrays or glDrawElements if
		any two active samplers in the current program object are of different
		types, but refer to the same texture image unit.
	GL_INVALID_OPERATION is generated if a geometry shader is active and mode
		is incompatible with the input primitive type of the geometry shader in
		the currently installed program object.
	GL_INVALID_OPERATION is generated if mode is GL_PATCHES and no tessellation
		control shader is active.
	GL_INVALID_OPERATION is generated if recording the vertices of a primitive
		to the buffer objects being used for transform feedback purposes would
		result in either exceeding the limits of any buffer object’s size, or
		in exceeding the end position offset + size - 1, as set by
		glBindBufferRange.
	GL_INVALID_OPERATION is generated by glDrawArrays() if no geometry shader
		is present, transform feedback is active and mode is not one of the
		allowed modes.
	GL_INVALID_OPERATION is generated by glDrawArrays() if a geometry shader is
		present, transform feedback is active and the output primitive type of
		the geometry shader does not match the transform feedback primitiveMode.
	GL_INVALID_OPERATION is generated if the bound shader program is invalid.
	GL_INVALID_OPERATION is generated if transform feedback is in use, and the
		buffer bound to the transform feedback binding point is also bound to
		the array buffer binding point.

	Own experience:
	GL_INVALID_OPERATION may be generated (or the driver crashes) if an enabled
		vertex attribute has no buffer attached and no pointer set.
*/
}

void GlCommandSink::Draw(TransformFeedback* transformFeedback, IndexBufferInfo::Mode mode)
{
	if(gl.HasDrawTransformFeedback())
	{
		gl.DrawTransformFeedback(ToGlEnum(mode), transformFeedback->mFeedback);
		CheckError("glDrawTransformFeedback", __LINE__);
	}
	else
	{
		// Query
		GLuint primitives = 0;
		gl.GetQueryObjectuiv(transformFeedback->mQuery, gl.QUERY_RESULT, &primitives);
		CheckError("glGetQueryObjectiv", __LINE__);
		gl.DrawArrays(ToGlEnum(mode), 0, primitives);
		CheckError("glDrawArrays", __LINE__);
	}
}

void GlCommandSink::Draw(IndexBufferInfo::Mode mode, unsigned int count)
{
	gl.DrawArrays(ToGlEnum(mode), 0, count);
	CheckError("glDrawArrays", __LINE__);
}

GLenum GlCommandSink::ToGlEnum(VertexAttributeInfo::Type type)
{
	switch(type)
	{
		case VertexAttributeInfo::kFloat: return gl.FLOAT;
		case VertexAttributeInfo::kInt8: return gl.BYTE;
		case VertexAttributeInfo::kUInt8: return gl.UNSIGNED_BYTE;
		case VertexAttributeInfo::kInt16: return gl.SHORT;
		case VertexAttributeInfo::kUInt16: return gl.UNSIGNED_SHORT;
		case VertexAttributeInfo::kInt32: return gl.INT;
		case VertexAttributeInfo::kUInt32: return gl.UNSIGNED_INT;
		case VertexAttributeInfo::kHalf: return gl.HALF_FLOAT;
	}
	return 0;
}

GLenum GlCommandSink::ToGlEnum(IndexBufferInfo::Type type)
{
	switch(type)
	{
		case IndexBufferInfo::Type::kUInt8: return gl.UNSIGNED_BYTE;
		case IndexBufferInfo::Type::kUInt16: return gl.UNSIGNED_SHORT;
		case IndexBufferInfo::Type::kUInt32: return gl.UNSIGNED_INT;
	}
	return 0;
}

GLenum GlCommandSink::ToGlEnum(IndexBufferInfo::Mode mode)
{
	switch(mode)
	{
		case IndexBufferInfo::Mode::kPoints: return GL_POINTS;
		case IndexBufferInfo::Mode::kTriangles: return GL_TRIANGLES;
		case IndexBufferInfo::Mode::kLines: return GL_LINES;
		case IndexBufferInfo::Mode::kTriangleFan: return GL_TRIANGLE_FAN;
		case IndexBufferInfo::Mode::kTriangleStrip: return GL_TRIANGLE_STRIP;
		case IndexBufferInfo::Mode::kLineStrip: return GL_LINE_STRIP;
		case IndexBufferInfo::Mode::kTrianglesAdjacency: return gl.TRIANGLES_ADJACENCY;
		case IndexBufferInfo::Mode::kTriangleStripAdjacency: return gl.TRIANGLE_STRIP_ADJACENCY;
		case IndexBufferInfo::Mode::kLineStripAdjacency: return gl.LINE_STRIP_ADJACENCY;
	}
	return 0;
}


/*****************************************************************************/

void GlCommandSink::Texture::Store(unsigned int width, unsigned int height, const void* data, PixelFormat format, int mipmapLevel, size_t dataSize)
{
	gl.BindTexture(GL_TEXTURE_2D, mTexture);
	CheckError("glBindTexture", __LINE__);
	GLenum intFormat = PixelFormatConversion::ToGlInternalFormat(format);
	if(Pf::IsCompressed(format))
	{
		gl.CompressedTexImage2D(GL_TEXTURE_2D, mipmapLevel, intFormat, width, height, 0, dataSize, data);
		CheckError("glCompressedTexImage2D", __LINE__);
	}
	else
	{
		// Uncompressed
		GLenum glFormat = PixelFormatConversion::ToGlFormat(format);
		GLenum type = PixelFormatConversion::ToGlType(format);
		if(width == mWidth && height == mHeight && mDepth == 0 && data)
			gl.TexSubImage2D(GL_TEXTURE_2D, mipmapLevel, 0, 0, width, height, glFormat, type, data);
		else
			gl.TexImage2D(GL_TEXTURE_2D, mipmapLevel, intFormat, width, height, 0, glFormat, type, data);
		CheckError("glTexImage2D", __LINE__);
	}
	mWidth = width;
	mHeight = height;
	mDepth = 0;
}

void GlCommandSink::Texture::Store(unsigned int width, unsigned int height, unsigned int depth, const void* data, PixelFormat format)
{
	gl.BindTexture(gl.TEXTURE_3D, mTexture);
	CheckError("glBindTexture", __LINE__);
	GLenum glFormat = PixelFormatConversion::ToGlFormat(format);
	GLenum intFormat = PixelFormatConversion::ToGlInternalFormat(format);
	GLenum type = PixelFormatConversion::ToGlType(format);
	gl.TexImage3D(gl.TEXTURE_3D, 0, intFormat, width, height, depth, 0, glFormat, type, data);
	CheckError("glTexImage3D", __LINE__);
	mWidth = width;
	mHeight = height;
	mDepth = depth;
}

void GlCommandSink::Texture::Store(unsigned int offsetX, unsigned int offsetY, unsigned int width, unsigned int height, const void* data, PixelFormat format, int mipmapLevel, size_t dataSize)
{
	gl.BindTexture(GL_TEXTURE_2D, mTexture);
	CheckError("glBindTexture", __LINE__);

	if(Pf::IsCompressed(format))
	{
		GLenum internalFormat = PixelFormatConversion::ToGlInternalFormat(format);
		gl.CompressedTexSubImage2D(GL_TEXTURE_2D, mipmapLevel, offsetX, offsetY, width, height, internalFormat, dataSize, data);
		CheckError("glCompressedTexSubImage2D", __LINE__);
	}
	else
	{
		GLenum glFormat = PixelFormatConversion::ToGlFormat(format);
		GLenum type = PixelFormatConversion::ToGlType(format);
		gl.TexSubImage2D(GL_TEXTURE_2D, mipmapLevel, offsetX, offsetY, width, height, glFormat, type, data);
		CheckError("glTexSubImage2D", __LINE__);
	}
}

void GlCommandSink::Texture::SetParameter(Parameter param, ParamValue value)
{
	if(mDepth > 0)
		gl.BindTexture(gl.TEXTURE_3D, mTexture);
	else
		gl.BindTexture(GL_TEXTURE_2D, mTexture);
	CheckError("glBindTexture", __LINE__, __FILE__);
	gl.TexParameteri(GL_TEXTURE_2D, param, value);
	CheckError("glTexParameteri", __LINE__, __FILE__);
}

void GlCommandSink::Texture::SetParameter(Parameter param, int value)
{
	if(mDepth > 0)
		gl.BindTexture(gl.TEXTURE_3D, mTexture);
	else
		gl.BindTexture(GL_TEXTURE_2D, mTexture);
	CheckError("glBindTexture", __LINE__, __FILE__);
	gl.TexParameteri(GL_TEXTURE_2D, param, value);
	CheckError("glTexParameteri", __LINE__, __FILE__);
}


/*****************************************************************************/

void GlCommandSink::RenderTarget::AttachColorBuffer(Texture* texture, unsigned int attachment, unsigned int layer)
{
	if(mColorRenderbuffers.size() > attachment && mColorRenderbuffers[attachment] != 0)
	{
		gl.DeleteRenderbuffers(1, &mColorRenderbuffers[attachment]);
		CheckError("glDeleteRenderbuffers", __LINE__);
		mColorRenderbuffers[attachment] = 0;
	}

	gl.BindFramebuffer(gl.FRAMEBUFFER, mFramebuffer);
	CheckError("glBindFramebuffer", __LINE__);
	if(texture)
	{
		if(texture->GetDepth() > 0)
		{
			assert(texture->GetDepth() > layer);
			gl.FramebufferTexture3D(gl.FRAMEBUFFER, gl.COLOR_ATTACHMENT0 + attachment, gl.TEXTURE_3D, texture->mTexture, 0, layer);
		}
		else
			gl.FramebufferTexture2D(gl.FRAMEBUFFER, gl.COLOR_ATTACHMENT0 + attachment, GL_TEXTURE_2D, texture->mTexture, 0);

		CheckError("glFramebufferTexture2D", __LINE__);
		mWidth = texture->GetWidth();
		mHeight = texture->GetHeight();
	}
	else // Detach texture
	{
		gl.FramebufferTexture2D(gl.FRAMEBUFFER, gl.COLOR_ATTACHMENT0 + attachment, GL_TEXTURE_2D, 0, 0);
		CheckError("glFramebufferTexture2D", __LINE__);
	}

	if(attachment == 0)
		mColorBufferAttached = (texture != nullptr);
}

void GlCommandSink::RenderTarget::AttachColorBuffer(unsigned int width, unsigned int height, PixelFormat format, unsigned int attachment)
{
	if(mColorRenderbuffers.size() <= attachment)
		mColorRenderbuffers.resize(attachment + 1);
	if(mColorRenderbuffers[attachment] == 0)
	{
		gl.GenRenderbuffers(1, &mColorRenderbuffers[attachment]);
		CheckError("glGenRenderbuffers", __LINE__);
	}
	gl.BindRenderbuffer(gl.RENDERBUFFER, mColorRenderbuffers[attachment]);
	CheckError("glBindRenderbuffer", __LINE__);
	gl.RenderbufferStorage(gl.RENDERBUFFER, PixelFormatConversion::ToGlInternalFormat(format), width, height);
	CheckError("glRenderbufferStorage", __LINE__);
	gl.BindFramebuffer(gl.FRAMEBUFFER, mFramebuffer);
	CheckError("glBindFramebuffer", __LINE__);
	gl.FramebufferRenderbuffer(gl.FRAMEBUFFER, gl.COLOR_ATTACHMENT0 + attachment, gl.RENDERBUFFER, mColorRenderbuffers[attachment]);
	CheckError("glFramebufferRenderbuffer", __LINE__);
	mColorBufferAttached = (attachment == 0);
	mWidth = width;
	mHeight = height;
}

void GlCommandSink::RenderTarget::AttachDepthBuffer(Texture* texture)
{
	if(mDepthRenderbuffer)
	{
		gl.DeleteRenderbuffers(1, &mDepthRenderbuffer);
		mDepthRenderbuffer = 0;
	}
	gl.BindFramebuffer(gl.FRAMEBUFFER, mFramebuffer);
	CheckError("glBindFramebuffer", __LINE__);

	if(texture)
	{
		gl.FramebufferTexture2D(gl.FRAMEBUFFER, gl.DEPTH_ATTACHMENT, GL_TEXTURE_2D, texture->mTexture, 0);
		mWidth = texture->GetWidth();
		mHeight = texture->GetHeight();
	}
	else
		gl.FramebufferTexture2D(gl.FRAMEBUFFER, gl.DEPTH_ATTACHMENT, GL_TEXTURE_2D, 0, 0);
	CheckError("glFramebufferTexture2D", __LINE__);

}

void GlCommandSink::RenderTarget::AttachDepthBuffer(unsigned int width, unsigned int height, PixelFormat format)
{
	if(!mDepthRenderbuffer)
		gl.GenRenderbuffers(1, &mDepthRenderbuffer);
	gl.BindRenderbuffer(gl.RENDERBUFFER, mDepthRenderbuffer);
	gl.RenderbufferStorage(gl.RENDERBUFFER, PixelFormatConversion::ToGlInternalFormat(format), width, height);
	gl.BindFramebuffer(gl.FRAMEBUFFER, mFramebuffer);
	gl.FramebufferRenderbuffer(gl.FRAMEBUFFER, gl.DEPTH_ATTACHMENT, gl.RENDERBUFFER, mDepthRenderbuffer);
	mWidth = width;
	mHeight = height;
}

void GlCommandSink::RenderTarget::AttachStencilBuffer(Texture* texture)
{
	if(mStencilRenderbuffer)
	{
		gl.DeleteRenderbuffers(1, &mStencilRenderbuffer);
		mStencilRenderbuffer = 0;
	}

	gl.BindFramebuffer(gl.FRAMEBUFFER, mFramebuffer);
	if(texture)
	{
		gl.FramebufferTexture2D(gl.FRAMEBUFFER, gl.STENCIL_ATTACHMENT, GL_TEXTURE_2D, texture->mTexture, 0);
		mWidth = texture->GetWidth();
		mHeight = texture->GetHeight();
	}
	else
		gl.FramebufferTexture2D(gl.FRAMEBUFFER, gl.STENCIL_ATTACHMENT, GL_TEXTURE_2D, 0, 0);

}

void GlCommandSink::RenderTarget::AttachStencilBuffer(unsigned int width, unsigned int height, PixelFormat format)
{
	if(!mStencilRenderbuffer)
		gl.GenRenderbuffers(1, &mStencilRenderbuffer);
	gl.BindRenderbuffer(gl.RENDERBUFFER, mStencilRenderbuffer);
	gl.RenderbufferStorage(gl.RENDERBUFFER, PixelFormatConversion::ToGlInternalFormat(format), width, height);
	gl.BindFramebuffer(gl.FRAMEBUFFER, mFramebuffer);
	gl.FramebufferRenderbuffer(gl.FRAMEBUFFER, gl.STENCIL_ATTACHMENT, gl.RENDERBUFFER, mStencilRenderbuffer);
	mWidth = width;
	mHeight = height;
}

GlCommandSink::RenderTarget::RenderTarget() :
	mStencilRenderbuffer(0),
	mDepthRenderbuffer(0),
	mColorBufferAttached(false),
	mWidth(0),
	mHeight(0)
{
	gl.GenFramebuffers(1, &mFramebuffer);
	CheckError("glGenFramebuffers", __LINE__);
}


GlCommandSink::RenderTarget::~RenderTarget()
{
	for(auto it: mColorRenderbuffers)
	{
		if(it != 0)
			gl.DeleteRenderbuffers(1, &it);
	}

	if(mDepthRenderbuffer)
		gl.DeleteRenderbuffers(1, &mDepthRenderbuffer);

	if(mStencilRenderbuffer)
		gl.DeleteRenderbuffers(1, &mStencilRenderbuffer);

	gl.DeleteFramebuffers(1, &mFramebuffer);
}

}
