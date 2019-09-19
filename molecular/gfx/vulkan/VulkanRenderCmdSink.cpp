/*	VulkanRenderCmdSink.cpp
	Copyright 2017 Fabian Herb

	This file is part of Molecular Engine.
*/

#include "VulkanRenderCmdSink.h"

VulkanRenderCmdSink::~VulkanRenderCmdSink()
{
}

void VulkanRenderCmdSink::Init()
{
}

void VulkanRenderCmdSink::VertexBuffer::Store(const void* data, size_t size, bool stream)
{}

void VulkanRenderCmdSink::IndexBuffer::Store(const void* data, size_t size)
{

}

VulkanRenderCmdSink::IndexBuffer* VulkanRenderCmdSink::CreateIndexBuffer()
{
	return nullptr;
}

VulkanRenderCmdSink::Texture* VulkanRenderCmdSink::CreateTexture()
{
	return nullptr;
}

void VulkanRenderCmdSink::DestroyTexture(Texture* texture)
{
	delete texture;
}


VulkanRenderCmdSink::Program* VulkanRenderCmdSink::CreateProgram()
{
	return nullptr;
}

void VulkanRenderCmdSink::DestroyProgram(VulkanRenderCmdSink::Program* program)
{
	delete program;
}

void VulkanRenderCmdSink::UseProgram(VulkanRenderCmdSink::Program* program)
{

}

VulkanRenderCmdSink::RenderTarget* VulkanRenderCmdSink::CreateRenderTarget()
{
	return nullptr;
}

void VulkanRenderCmdSink::DestroyRenderTarget(RenderTarget* target)
{
	delete target;
}

void VulkanRenderCmdSink::SetTarget(VulkanRenderCmdSink::RenderTarget* target)
{

}

void VulkanRenderCmdSink::SetBaseTarget(const IntVector4& viewport, int renderTarget)
{

}

void VulkanRenderCmdSink::TransformFeedback::SetDestination(VulkanRenderCmdSink::VertexBuffer* destinationBuffer)
{

}

void VulkanRenderCmdSink::TransformFeedback::Begin(IndexBufferInfo::Mode mode)
{

}

void VulkanRenderCmdSink::TransformFeedback::End()
{

}

VulkanRenderCmdSink::TransformFeedback* VulkanRenderCmdSink::CreateTransformFeedback()
{
	return nullptr;
}

void VulkanRenderCmdSink::SetBackgroundColor(float r, float g, float b, float a)
{

}

void VulkanRenderCmdSink::Clear(bool color, bool depth)
{

}

void VulkanRenderCmdSink::Draw(VulkanRenderCmdSink::IndexBuffer* buffer, const IndexBufferInfo& info)
{

}

void VulkanRenderCmdSink::Draw(VulkanRenderCmdSink::TransformFeedback* transformFeedback, IndexBufferInfo::Mode mode)
{

}

void VulkanRenderCmdSink::Draw(IndexBufferInfo::Mode mode, unsigned int count)
{

}

void VulkanRenderCmdSink::ReadPixels(int x, int y, int width, int height, PixelFormat format, void* data)
{

}

void VulkanRenderCmdSink::SetBlending(bool enable, VulkanRenderCmdSink::BlendFactor sourceFactor, VulkanRenderCmdSink::BlendFactor destFactor)
{

}

void VulkanRenderCmdSink::SetDepthState(bool depthTest, bool depthWrite, CompareOp compareOp)
{

}

void  VulkanRenderCmdSink::SetScissor(bool enable, int x, int y, int width, int height)
{

}

void VulkanRenderCmdSink::SetRasterizationState(bool rasterizerDiscard, VulkanRenderCmdSink::CullMode cullMode)
{

}

void VulkanRenderCmdSink::Texture::Store(unsigned int width, unsigned int height, const void* data, PixelFormat format, int mipmapLevel, size_t dataSize)
{

}

void VulkanRenderCmdSink::Texture::Store(unsigned int width, unsigned int height, unsigned int depth, const void* data, PixelFormat format)
{

}

void VulkanRenderCmdSink::Texture::Store(unsigned int offsetX, unsigned int offsetY, unsigned int width, unsigned int height, const void* data, PixelFormat format, int mipmapLevel, size_t dataSize)
{

}

void VulkanRenderCmdSink::RenderTarget::AttachColorBuffer(Texture* texture, unsigned int attachment, unsigned int layer)
{

}

void VulkanRenderCmdSink::RenderTarget::AttachColorBuffer(unsigned int width, unsigned int height, PixelFormat format, unsigned int attachment)
{

}

void VulkanRenderCmdSink::RenderTarget::AttachDepthBuffer(Texture* texture)
{

}

void VulkanRenderCmdSink::RenderTarget::AttachDepthBuffer(unsigned int width, unsigned int height, PixelFormat format)
{

}

void VulkanRenderCmdSink::RenderTarget::AttachStencilBuffer(Texture* texture)
{

}

void VulkanRenderCmdSink::RenderTarget::AttachStencilBuffer(unsigned int width, unsigned int height, PixelFormat format)
{

}
