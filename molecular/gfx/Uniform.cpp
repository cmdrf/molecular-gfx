/*	Uniform.cpp

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

#include "Uniform.h"
#include <assert.h>

namespace molecular
{
namespace gfx
{

Variable::~Variable()
{

}


Attribute::Attribute() :
	mComponents(0),
	mOffset(0),
	mStride(0)
{

}

Attribute::Attribute(RenderCmdSink::VertexBuffer* buffer, const VertexAttributeInfo& info) :
	mBuffer(std::move(buffer)),
	mType(info.type),
	mComponents(info.components),
	mOffset(info.offset),
	mStride(info.stride)
{

}

Attribute::Attribute(RenderCmdSink::VertexBuffer* buffer, int components, VertexAttributeInfo::Type type, int stride, int offset) :
	mBuffer(std::move(buffer)),
	mType(type),
	mComponents(components),
	mOffset(offset),
	mStride(stride)
{

}

void Attribute::Apply(RenderCmdSink::Program* program, Hash name) const
{
//	std::cerr << "program->SetAttribute(..., " << mComponents << ", " << mType << ", " << mStride << ", " << mOffset <<")" << std::endl;
	assert(program);
	if(mBuffer)
		program->SetAttribute(name, mBuffer, mComponents, mType, mStride, mOffset);
}

}
}
