/*	Uniform.h
	Copyright 2012-2017 Fabian Herb

	This file is part of Molecular Engine.
*/

#ifndef UNIFORM_H
#define UNIFORM_H

#include "RenderCmdSink.h"
#include <assert.h>

namespace molecular
{
namespace Gfx
{

/// Variable that can be fed to Renderer::Program
class Variable
{
public:
	virtual ~Variable();

	/// Feed to program
	virtual void Apply(RenderCmdSink::Program* program, Hash name) const = 0;

	virtual unsigned int GetArraySize() const {return 0;}
};

/// Uniform variable that can be fed to a Renderer::Program
template<class T, int arraySize = 1>
class Uniform : public Variable
{
public:
	void Apply(RenderCmdSink::Program* program, Hash name) const override
	{
		assert(program);
		program->SetUniform(name, mValues, arraySize);
	}
//	using Variable::Apply;

	unsigned int GetArraySize() const override {return arraySize == 1 ? 0 : arraySize;}

	T& operator*() {return mValues[0];}
	const T& operator*() const {return mValues[0];}
	T& operator[] (int i) {return mValues[i];}
	const T& operator[] (int i) const {return mValues[i];}

private:
	T mValues[arraySize];
};

/// Vertex attribute variable
/** Points to a Renderer::VertexBuffer and includes information on how to
	interpret the data. */
class Attribute : public Variable
{
public:
	Attribute();

	/// Construct from buffer and info struct
	Attribute(RenderCmdSink::VertexBuffer* buffer, const VertexAttributeInfo& info);

	/// Construct from buffer and individual infos
	Attribute(RenderCmdSink::VertexBuffer* buffer, int components, VertexAttributeInfo::Type type = VertexAttributeInfo::kFloat, int stride = 0, int offset = 0);

	void Apply(RenderCmdSink::Program* program, Hash name) const override;

private:
	RenderCmdSink::VertexBuffer* mBuffer;
	VertexAttributeInfo::Type mType;

	/// Number of components per entry
	int mComponents;

	/// Offset to the first element in the buffer, measured in bytes
	int mOffset;

	/// Offset from one entry to the next
	/** Measured in bytes. 0 means entries are tightly packed. */
	int mStride;
};

/// Dummy variable for declaring shader outputs
/** This is where the ProgramGenerator starts building the function graph. */
class Output : public Variable
{
public:
	/** This is never called. */
	void Apply(RenderCmdSink::Program* /*program*/, Hash /*name*/) const override {}
};

}
}

#endif // UNIFORM_H
