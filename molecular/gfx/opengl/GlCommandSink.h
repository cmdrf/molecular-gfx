/*	GlCommandSink.h
	Copyright 2012-2017 Fabian Herb

	This file is part of Molecular Engine.
*/

#ifndef GLCOMMANDSINK_H
#define GLCOMMANDSINK_H

//#pragma warning(disable: 4267)

#include "GlFunctions.h"
#include <molecular/gfx/BufferInfo.h>
#include <molecular/util/NonCopyable.h>
#include <molecular/util/PixelFormat.h>
#include <molecular/util/Logging.h>
#include "GlConstantString.h"
#include <molecular/util/Vector.h>

namespace molecular
{

/// Interface to OpenGL
/** The constructor can be called outside the renderer thread. After construction, Init() must be
	called. Init() can only be called inside the renderer thread. */
class GlCommandSink : NonCopyable
{
private:
	/// Parent for VertexBuffer and IndexBuffer
	class Buffer : NonCopyable
	{
	protected:
		Buffer() = default;
		explicit Buffer(GLuint buffer) : mBuffer(buffer) {}
		~Buffer()
		{
			if(mBuffer > 0)
				gl.DeleteBuffers(1, &mBuffer);
		}

		GLuint mBuffer = 0;
	};

public:
	/// Initialise
	/** This must be called in the renderer thread. */
	void Init();

	/// Buffer storing vertex data
	/** @see CreateVertexBuffer */
	class VertexBuffer : public Buffer
	{
		friend class GlCommandSink;
	public:
		void Store(const void* data, size_t size, bool stream = false);

	private:
		VertexBuffer(GLuint buffer) : Buffer(buffer) {}
	};

	VertexBuffer* CreateVertexBuffer()
	{
		GLuint buffer;
		gl.GenBuffers(1, &buffer);
		return new VertexBuffer(buffer);
	}
	void DestroyVertexBuffer(VertexBuffer* buffer) {delete buffer;}

	/// Buffer storing index data
	class IndexBuffer : public Buffer
	{
		friend class GlCommandSink;
	public:
		void Store(const void* data, size_t size);

	private:
		IndexBuffer(GLuint buffer) : Buffer(buffer) {}
	};

	/// Create IndexBuffer
	IndexBuffer* CreateIndexBuffer()
	{
		GLuint buffer;
		gl.GenBuffers(1, &buffer);
		return new IndexBuffer(buffer);
	}
	void DestroyIndexBuffer(IndexBuffer* buffer) {delete buffer;}

	class Texture;

	Texture* CreateTexture();
	void DestroyTexture(Texture* texture);

	class Program;

	/// Create shader program
	/** Call DestroyProgram after being finished with the program.
		@see DestroyProgram
		@todo Return move-only type. */
	Program* CreateProgram();

	/// Destroy shader program
	/** @see CreateProgram */
	void DestroyProgram(Program* program);

	/// Make program current
	/** Also call before setting uniforms. */
	void UseProgram(Program* program);

	class RenderTarget;

	RenderTarget* CreateRenderTarget();
	void DestroyRenderTarget(RenderTarget* target);

	/// Set render target
	/** Pass nullptr to render to the base target. The base target can be specified with
		SetBaseTarget. */
	void SetTarget(RenderTarget* target);
	RenderTarget* GetTarget() {return mCurrentTarget;}

	/** Registers the currently bound framebuffer as the base target. In the simplest case this
		will be 0, which is the screen. */
	void SetBaseTarget(const IntVector4& viewport, int renderTarget);

	class TransformFeedback;

	TransformFeedback* CreateTransformFeedback();
	void DestroyTransformFeedback(TransformFeedback* feedback);

	void SetBackgroundColor(float r, float g, float b, float a = 1.0f) {gl.ClearColor(r, g, b, a);}

	/// Clear current render target.
	void Clear(bool color = true, bool depth = true);

	/// Draw indices
	/** Requires an active program.
		@see UseProgram */
	void Draw(IndexBuffer* buffer, const IndexBufferInfo& info);

	void Draw(TransformFeedback* transformFeedback, IndexBufferInfo::Mode mode);

	/// Draw vertices without indices
	/** Requires active program. */
	void Draw(IndexBufferInfo::Mode mode, unsigned int count);

	void ReadPixels(int x, int y, int width, int height, PixelFormat format, void* data);

	enum BlendFactor
	{
		kOne = GL_ONE,
		kSrcAlpha = GL_SRC_ALPHA,
		kOneMinusSrcAlpha = GL_ONE_MINUS_SRC_ALPHA
	};

	void SetBlending(bool enable, BlendFactor sourceFactor = kSrcAlpha, BlendFactor destFactor = kOneMinusSrcAlpha);

	enum CompareOp
	{
		kNever = GL_NEVER,
		kLess = GL_LESS,
		kEqual = GL_EQUAL,
		kLessOrEqual = GL_LEQUAL,
		kGreater = GL_GREATER,
		kNotEqual = GL_NOTEQUAL,
		kGreaterOrEqual = GL_GEQUAL,
		kAlways = GL_ALWAYS
	};

	void SetDepthState(bool depthTest, bool depthWrite, CompareOp compareOp = kLess);
	void SetScissor(bool enable, int x = 0, int y = 0, int width = 0, int height = 0);

	enum CullMode
	{
		kFront = GL_FRONT,
		kBack = GL_BACK
	};

	void SetRasterizationState(bool rasterizerDiscard, CullMode cullMode = kBack);

	static inline void CheckError(const char* function, int line, const char* file = "Renderer.cpp")
	{
#if 1 //ndef NDEBUG
//		std::cerr << function << "(" << file << ":" << line << ")" << std::endl;
		GLenum error = glGetError();
		if(error)
		{
			LOG(ERROR) << std::dec << file << ":" << line << ": " << function << ": " << GlConstantString(error);
		}
#endif
	}

	/** @todo Private? Global? Singleton? Thread local? */
	static GlFunctions gl;

private:
	static GLenum ToGlEnum(VertexAttributeInfo::Type type);
	static GLenum ToGlEnum(IndexBufferInfo::Type type);
	static GLenum ToGlEnum(IndexBufferInfo::Mode mode);

	RenderTarget* mCurrentTarget = nullptr;
	GLuint mBaseTargetFramebuffer = 0;
	molecular::IntVector4 mBaseTargetViewport = {0, 0, 640, 480};
};

/******************************* Nested classes ******************************/

/// Texture object
class GlCommandSink::Texture : NonCopyable
{
	friend class GlCommandSink;
public:
	/// Store texture data
	/** @param data Pointer to the texture data. Can be NULL to allocate
			but not initialize the texture.
		@param dataSize Size of the data block in bytes. Only needed for
			compressed textures. */
	void Store(unsigned int width, unsigned int height, const void* data, PixelFormat format, int mipmapLevel = 0, size_t dataSize = 0);

	/// Store 3D texture data
	void Store(unsigned int width, unsigned int height, unsigned int depth, const void* data, PixelFormat format);

	/// Updates a portion of the image
	void Store(unsigned int offsetX, unsigned int offsetY, unsigned int width, unsigned int height, const void* data, PixelFormat format, int mipmapLevel = 0, size_t dataSize = 0);
	void GenerateMipmaps() {gl.BindTexture(GL_TEXTURE_2D, mTexture); gl.GenerateMipmap(GL_TEXTURE_2D);}

	enum Parameter
	{
#ifndef OPENGL_ES3
		kBorderColor = gl.TEXTURE_BORDER_COLOR,
		kLodBias = gl.TEXTURE_LOD_BIAS,

#endif
		kBaseLevel = gl.TEXTURE_BASE_LEVEL,
		kCompareFunc = gl.TEXTURE_COMPARE_FUNC,
		kCompareMode = gl.TEXTURE_COMPARE_MODE,
		kMinLod = gl.TEXTURE_MIN_LOD,
		kMaxLod = gl.TEXTURE_MAX_LOD,
		kMaxLevel = gl.TEXTURE_MAX_LEVEL,
		kWrapR = gl.TEXTURE_WRAP_R,
		kMinFilter = gl.TEXTURE_MIN_FILTER,
		kMagFilter = gl.TEXTURE_MAG_FILTER,
		kWrapS = gl.TEXTURE_WRAP_S,
		kWrapT = gl.TEXTURE_WRAP_T
	};

	enum ParamValue
	{
		kLinear = GL_LINEAR,
		kNearest = GL_NEAREST,
		kNearestMipmapLinear = GL_NEAREST_MIPMAP_LINEAR,
		kLinearMipmapLinear = GL_LINEAR_MIPMAP_LINEAR,
		kLinearMipmapNearest = GL_LINEAR_MIPMAP_NEAREST,
		kLessEqual = GL_LEQUAL,
		kCompareRefToTexture = gl.COMPARE_REF_TO_TEXTURE,
		kClampToEdge = gl.CLAMP_TO_EDGE,
		kRepeat = GL_REPEAT
	};

	void SetParameter(Parameter param, ParamValue value);
	void SetParameter(Parameter param, int value);

	unsigned int GetWidth() const {return mWidth;}
	unsigned int GetHeight() const {return mHeight;}
	unsigned int GetDepth() const {return mDepth;}

private:
	Texture() : mWidth(0), mHeight(0), mDepth(0)
	{
		gl.GenTextures(1, &mTexture);
		GlCommandSink::CheckError("glGenTextures", __LINE__, __FILE__);
	}
	~Texture() {gl.DeleteTextures(1, &mTexture);}

	GLuint mTexture;
	unsigned int mWidth, mHeight, mDepth;
};

/// Allows for rendering to textures
/** @see CreateRenderTarget
	@see DestroyRenderTarget */
class GlCommandSink::RenderTarget : NonCopyable
{
	friend class GlCommandSink;
public:
	/// Set texture to render color to
	/** @param texture The texture to render to.
		@param attachment Use values different from 0 to activate MRTs.
			Not available on GLES.
		@param layer Layer of the 3D texture to draw to (only applicable to 3D textures). */
	void AttachColorBuffer(Texture* texture, unsigned int attachment = 0, unsigned int layer = 0);

	/// Enable rendering to a buffer
	/** The buffer cannot be used as a texture afterwards. Rarely needed for
		color targets. Simply don't output a fragmentColor when not needing
		color output. */
	void AttachColorBuffer(unsigned int width, unsigned int height, PixelFormat format, unsigned int attachment = 0);

	/// Set texture to render depth to
	/** Rendering depth to a texture requires OES_depth_texture extension on
		GLESv2. */
	void AttachDepthBuffer(Texture* texture);

	void AttachDepthBuffer(unsigned int width, unsigned int height, PixelFormat format);

	void AttachStencilBuffer(Texture* texture);
	void AttachStencilBuffer(unsigned int width, unsigned int height, PixelFormat format);

	bool ColorBufferAttached() const {return mColorBufferAttached;}

	unsigned int GetWidth() const {return mWidth;}
	unsigned int GetHeight() const {return mHeight;}

private:
	RenderTarget();
	~RenderTarget();

	GLuint mFramebuffer;
	GLuint mStencilRenderbuffer;
	GLuint mDepthRenderbuffer;
	std::vector<GLuint> mColorRenderbuffers;
	bool mColorBufferAttached;
	unsigned int mWidth, mHeight;
};

class GlCommandSink::TransformFeedback : NonCopyable
{
	friend class GlCommandSink;
public:
	void SetDestination(VertexBuffer* destinationBuffer)
	{
		gl.BindTransformFeedback(gl.TRANSFORM_FEEDBACK, mFeedback);
		CheckError("glBindTransformFeedback", __LINE__, __FILE__);
		gl.BindBufferBase(gl.TRANSFORM_FEEDBACK_BUFFER, 0, destinationBuffer->mBuffer);
		CheckError("glBindBufferBase", __LINE__, __FILE__);
	}

	void Begin(IndexBufferInfo::Mode mode)
	{
		gl.BindTransformFeedback(gl.TRANSFORM_FEEDBACK, mFeedback);
		CheckError("glBindTransformFeedback", __LINE__, __FILE__);
		gl.BeginTransformFeedback(ToGlEnum(mode));
		CheckError("glBeginTransformFeedback", __LINE__, __FILE__);
		if(mQuery)
		{
			gl.BeginQuery(gl.TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN, mQuery);
			CheckError("glBeginQuery", __LINE__, __FILE__);
		}
	}

	void End()
	{
		gl.EndTransformFeedback();
		CheckError("glEndTransformFeedback", __LINE__, __FILE__);
		if(mQuery)
		{
			gl.EndQuery(gl.TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN);
			CheckError("glEndQuery", __LINE__, __FILE__);
		}
	}

private:
	TransformFeedback(GLuint name) : mFeedback(name)
	{
		if(!gl.HasDrawTransformFeedback())
			gl.GenQueries(1, &mQuery);
	}

	~TransformFeedback()
	{
		gl.DeleteTransformFeedbacks(1, &mFeedback);
		if(mQuery)
			gl.DeleteQueries(1, &mQuery);
	}

	GLuint mFeedback = 0;
	GLuint mQuery = 0;
};

}

#endif // GLCOMMANDSINK_H
