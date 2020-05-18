/*	Uniform.h

MIT License

Copyright (c) 2019-2020 Fabian Herb

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

#ifndef MOLECULAR_UNIFORM_H
#define MOLECULAR_UNIFORM_H

#include "RenderCmdSink.h"
#include "TextureManager.h"

#include <molecular/util/Scope.h>
#include <assert.h>

namespace molecular
{
namespace gfx
{

/// %Variable that can be fed to Renderer::Program
class Variable
{
public:
	virtual ~Variable();

	/// Feed to program
	virtual void Apply(RenderCmdSink::Program* program, Hash name) const = 0;

	virtual unsigned int GetArraySize() const {return 0;}
};

/// %Uniform variable that can be fed to a Renderer::Program
template<class T, int arraySize = 1>
class Uniform : public Variable
{
public:
	Uniform() = default;
	Uniform(const T& value)
	{
		static_assert (arraySize == 1, "This constructor can only be used for non-array uniforms");
		mValues[0] = value;
	}

	void Apply(RenderCmdSink::Program* program, Hash name) const override
	{
		assert(program);
		program->SetUniform(name, mValues, arraySize);
	}

	unsigned int GetArraySize() const override {return arraySize == 1 ? 0 : arraySize;}

	T& operator*() {return mValues[0];}
	const T& operator*() const {return mValues[0];}
	T* operator->() {return &mValues[0];}
	const T* operator->() const {return &mValues[0];}
	T& operator[] (int i) {return mValues[i];}
	const T& operator[] (int i) const {return mValues[i];}

private:
	T mValues[arraySize];
};

template<class T>
Uniform<T> MakeUniform(const T& v)
{
	return Uniform<T>(v);
}

class TextureUniform : public Variable
{
public:
	TextureUniform() = default;
	explicit TextureUniform(Hash textureName, TextureManager& textureManager);

	void Apply(RenderCmdSink::Program* program, Hash name) const override;

	void SetParameter(GlCommandSink::Texture::Parameter param, GlCommandSink::Texture::ParamValue value);

private:
	TextureManager::Asset* mAsset = nullptr;
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

	/// Offset from the beginning of one entry to the next
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

#endif // MOLECULAR_UNIFORM_H
