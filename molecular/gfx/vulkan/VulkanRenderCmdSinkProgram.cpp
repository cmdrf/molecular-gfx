/*	VulkanRenderCmdSinkProgram.cpp

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

#include "VulkanRenderCmdSinkProgram.h"

namespace molecular
{

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

}
