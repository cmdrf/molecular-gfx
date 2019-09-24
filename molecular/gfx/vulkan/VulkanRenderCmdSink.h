/*	VulkanRenderCmdSink.h

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

#ifndef MOLECULAR_VULKANRENDERCMDSINK_H
#define MOLECULAR_VULKANRENDERCMDSINK_H

#include <vector>
#include <list>
#include <molecular/util/BufferInfo.h>
#include <molecular/util/NonCopyable.h>
#include <molecular/util/PixelFormat.h>
#include <molecular/util/Logging.h>
#include <molecular/gfx/RenderContext.h>
#include <vulkan/vulkan.hpp>

namespace molecular
{

/// Interface to Vulkan
class VulkanRenderCmdSink : NonCopyable
{
public:
	~VulkanRenderCmdSink();

	/// Initialise
	/** This must be called in the renderer thread. */
	void Init();

	/// Buffer storing vertex data
	/** @see CreateVertexBuffer */
	class VertexBuffer
	{
	public:
		void Store(const void* data, size_t size, bool stream = false);
	};

	VertexBuffer* CreateVertexBuffer() {return nullptr;}
	void DestroyVertexBuffer(VertexBuffer* buffer) {delete buffer;}

	/// Buffer storing index data
	class IndexBuffer
	{
	public:
		void Store(const void* data, size_t size);
	};

	/// Create IndexBuffer
	IndexBuffer* CreateIndexBuffer();
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
	RenderTarget* GetTarget() {return nullptr;}

	/** Registers the currently bound framebuffer as the base target. In the simplest case this
		will be 0, which is the screen. */
	void SetBaseTarget(const IntVector4& viewport, int renderTarget);

	class TransformFeedback : NonCopyable
	{
	public:
		void SetDestination(VertexBuffer* destinationBuffer);

		void Begin(IndexBufferInfo::Mode mode);
		void End();
	};

	TransformFeedback* CreateTransformFeedback();
	void DestroyTransformFeedback(TransformFeedback* feedback) {delete feedback;}

	void SetBackgroundColor(float r, float g, float b, float a = 1.0f);

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
		kOne,
		kSrcAlpha,
		kOneMinusSrcAlpha
	};

	void SetBlending(bool enable, BlendFactor sourceFactor = kSrcAlpha, BlendFactor destFactor = kOneMinusSrcAlpha);

	enum CompareOp
	{
		kNever = VK_COMPARE_OP_NEVER,
		kLess = VK_COMPARE_OP_LESS,
		kEqual = VK_COMPARE_OP_EQUAL,
		kLessOrEqual = VK_COMPARE_OP_LESS_OR_EQUAL,
		kGreater = VK_COMPARE_OP_GREATER,
		kNotEqual = VK_COMPARE_OP_NOT_EQUAL,
		kGreaterOrEqual = VK_COMPARE_OP_GREATER_OR_EQUAL,
		kAlways = VK_COMPARE_OP_ALWAYS
	};

	void SetDepthState(bool depthTest, bool depthWrite, CompareOp compareOp = kLess);
	void SetScissor(bool enable, int x = 0, int y = 0, int width = 0, int height = 0);

	enum CullMode
	{
		kFront,
		kBack
	};

	void SetRasterizationState(bool rasterizerDiscard, CullMode cullMode = kBack);

private:
};

/******************************* Nested classes ******************************/

/// Texture object
class VulkanRenderCmdSink::Texture : NonCopyable
{
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
	void GenerateMipmaps() {}

	enum Parameter
	{
		kBorderColor,
		kLodBias,
		kBaseLevel,
		kCompareFunc,
		kCompareMode,
		kMinLod,
		kMaxLod,
		kMaxLevel,
		kWrapR,
		kMinFilter,
		kMagFilter,
		kWrapS,
		kWrapT
	};

	enum ParamValue
	{
		kLinear,
		kNearest,
		kNearestMipmapLinear,
		kLinearMipmapLinear,
		kLinearMipmapNearest,
		kLessEqual,
		kCompareRefToTexture,
		kClampToEdge,
		kRepeat
	};

	void SetParameter(Parameter param, ParamValue value) {}
	void SetParameter(Parameter param, int value) {}

	unsigned int GetWidth() {}
	unsigned int GetHeight() {}
	unsigned int GetDepth() {}

private:
	VkSampler mSampler = nullptr;

	VkImage mImage = nullptr;
	VkImageLayout mImageLayout = VK_IMAGE_LAYOUT_UNDEFINED;

	VkDeviceMemory mMemory = nullptr;
	VkImageView mView = nullptr;
	int32_t mWidth = 0;
	int32_t mHeight = 0;
};

/// Allows for rendering to textures
/** @see CreateRenderTarget
	@see DestroyRenderTarget */
class VulkanRenderCmdSink::RenderTarget : NonCopyable
{
public:
	/// Set texture to render color to
	/** @param texture The texture to render to.
		@param attachment Use values different from 0 to activate MRTs.
		@param layer Layer of the 3D texture to draw to (only applicable to 3D textures). */
	void AttachColorBuffer(Texture* texture, unsigned int attachment = 0, unsigned int layer = 0);

	/// Enable rendering to a buffer
	/** The buffer cannot be used as a texture afterwards. Rarely needed for
		color targets. Simply don't output a fragmentColor when not needing
		color output. */
	void AttachColorBuffer(unsigned int width, unsigned int height, PixelFormat format, unsigned int attachment = 0);

	/// Set texture to render depth to
	void AttachDepthBuffer(Texture* texture);

	void AttachDepthBuffer(unsigned int width, unsigned int height, PixelFormat format);

	void AttachStencilBuffer(Texture* texture);
	void AttachStencilBuffer(unsigned int width, unsigned int height, PixelFormat format);

	bool ColorBufferAttached() const {}

	unsigned int GetWidth() const {}
	unsigned int GetHeight() const {}
};

}

#endif // MOLECULAR_VULKANRENDERCMDSINK_H
