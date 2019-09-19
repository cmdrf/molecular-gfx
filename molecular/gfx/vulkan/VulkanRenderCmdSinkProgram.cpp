/*	VulkanRenderCmdSinkProgram.cpp
	Copyright 2017-2018 Fabian Herb

	This file is part of Molecular Engine.
*/

#include "VulkanRenderCmdSinkProgram.h"

void VulkanRenderCmdSink::Program::Store(const std::string& vertexShaderSource, const std::string& pixelShaderSource)
{
	ShaderSourceItem items[2] =
	{
		{kVertexShader, vertexShaderSource.data(), vertexShaderSource.size()},
		{kFragmentShader, pixelShaderSource.data(), pixelShaderSource.size()}
	};
	Store(items, 2);
}

void VulkanRenderCmdSink::Program::Store(const VulkanRenderCmdSink::Program::ShaderSourceItem items[], size_t count, const char** transformFeedbackOutputs, unsigned int tfOutputsCount)
{

}

void VulkanRenderCmdSink::Program::SetAttribute(Hash key, VertexBuffer* buffer, int components, VertexAttributeInfo::Type type, int stride, int offset, bool normalized)
{

}
