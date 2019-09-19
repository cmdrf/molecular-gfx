/*	Uniform.cpp
	Copyright 2014-2016 Fabian Herb

	This file is part of Molecular Engine.
*/

#include "Uniform.h"
#include <assert.h>

namespace molecular
{
namespace Gfx
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
