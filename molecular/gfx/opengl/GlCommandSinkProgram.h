/*	GlCommandSinkProgram.h

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

#ifndef MOLECULAR_GLCOMMANDSINKPROGRAM_H
#define MOLECULAR_GLCOMMANDSINKPROGRAM_H

#include "GlCommandSink.h"
#include <unordered_map>
#include <unordered_set>
#include <molecular/util/Vector4.h>
#include <molecular/util/Matrix.h>
#include <molecular/util/Hash.h>

#include <vector>

namespace molecular
{
namespace gfx
{
using namespace util;

/// Compiled shader program
/** @see Renderer::CreateProgram
	@see Renderer::DestroyProgram */
class GlCommandSink::Program : NonCopyable
{
	friend class GlCommandSink;
public:
	enum ShaderType
	{
		kFragmentShader = gl.FRAGMENT_SHADER,
		kVertexShader = gl.VERTEX_SHADER,
		kGeometryShader = gl.GEOMETRY_SHADER
	};

	struct ShaderSourceItem
	{
		ShaderType type;
		const char* source;
		size_t size;
	};

	/// Compile source code
	/** Legacy interface. */
	void Store(const std::string& vertexShaderSource, const std::string& pixelShaderSource);

	/// Compile source code
	void Store(const ShaderSourceItem items[], size_t count, const char** transformFeedbackOutputs = nullptr, unsigned int tfOutputsCount = 0);

	/// Get set of uniforms and attributes
	/** @see SetUniform
		@see SetAttribute */
	const std::unordered_set<Hash>& GetInputs() const {return mInputs;}

	/// Set uniform values
	/** @param key Hash of the uniform name,
		@param values Pointer to the first (or single) value of the array.
		@param count Elements in the array (or 1 for non-array uniforms). */
	inline void SetUniform(Hash key, const float* values, int count = 1) {gl.Uniform1fv(GetUniformLocation(key), count, values);}
	/** @copydoc SetUniform */
	inline void SetUniform(Hash key, const int* values, int count = 1) {gl.Uniform1iv(GetUniformLocation(key), count, values);}

	/** @copydoc SetUniform */
	inline void SetUniform(Hash key, const unsigned int values[], int count = 1) {gl.Uniform1uiv(GetUniformLocation(key), count, values);}

	/** @copydoc SetUniform */
	inline void SetUniform(Hash key, const Vector2* values, int count = 1);
	/** @copydoc SetUniform */
	inline void SetUniform(Hash key, const Vector3* values, int count = 1);
	/** @copydoc SetUniform */
	inline void SetUniform(Hash key, const Vector4* values, int count = 1);

	/** @copydoc SetUniform */
	inline void SetUniform(Hash key, const IntVector2* values, int count = 1);

	/** @copydoc SetUniform */
	inline void SetUniform(Hash key, const IntVector3* values, int count = 1);

	/** @copydoc SetUniform */
	inline void SetUniform(Hash key, const IntVector4* values, int count = 1);

	/** @copydoc SetUniform */
	inline void SetUniform(Hash key, const UIntVector2 values[], int count = 1);

	/** @copydoc SetUniform */
	inline void SetUniform(Hash key, const UIntVector3 values[], int count = 1);

	/** @copydoc SetUniform */
	inline void SetUniform(Hash key, const UIntVector4 values[], int count = 1);

	/** @copydoc SetUniform */
	inline void SetUniform(Hash key, const Matrix<3, 3>* values, int count = 1);

	/** @copydoc SetUniform */
	inline void SetUniform(Hash key, const Matrix<4, 4>* values, int count = 1);

	/// Set texture sampler uniform
	void SetUniform(Hash key, const Texture* const* textures, int count = 1);

	/// Set vertex attribute
	void SetAttribute(Hash key, VertexBuffer* buffer, int components, VertexAttributeInfo::Type type, int stride = 0, int offset = 0, bool normalized = true);

	void SetAttribute(Hash key, VertexBuffer* buffer, const VertexAttributeInfo& info)
	{
		SetAttribute(key, buffer, info.components, info.type, info.stride, info.offset, info.normalized);
	}

private:
	Program();
	~Program();

	class Shader : NonCopyable
	{
	public:
		explicit Shader(ShaderType type);
		Shader(Shader&& that) noexcept : mShader(that.mShader) {that.mShader = 0;}
		~Shader() {if(mShader) gl.DeleteShader(mShader);}

		bool SourceCompile(const char* text, size_t length);
		bool SourceCompile(const std::string& source) {return SourceCompile(source.data(), source.size());}

		GLuint mShader;
	};

//		inline GLint GetUniformLocation(const char* name) {return glGetUniformLocation(mProgram, name);}
	inline GLint GetUniformLocation(Hash hash) const;

	void Postprocess();

	/// Tells if the given uniform type is a texture sampler
	static bool IsSamplerType(GLenum uniformType);

	static bool IsIntType(GLenum type);

	GLuint mProgram;

	/** Not needed on ES2. */
	GLuint mVertexArrayObject;

	/// Maps uniform name hashes to uniform locations
	/** @see SetUniform */
	std::unordered_map<Hash, GLint> mUniformLocations;

	/// Uniforms and attributes
	std::unordered_set<Hash> mInputs;

	/// Texture Units
	/** The index of the vector element is the number of the texture unit. */
	std::vector<Hash> mTextureUnits;

	/// Attributes
	/** The index of the vector element is the attribute index. */
	std::vector<Hash> mAttributes;

	std::vector<bool> mAttributeIsInts;
};


/******************************* Implementation ******************************/

inline void GlCommandSink::Program::SetUniform(Hash key, const Vector2* values, int count)
{
	static_assert(sizeof(Vector2) == 2 * sizeof(GLfloat), "Vector2 seems to have additional fields");
	// All values consecutively in memory: pass pointer to first value
	gl.Uniform2fv(GetUniformLocation(key), count, values[0]);
}

inline void GlCommandSink::Program::SetUniform(Hash key, const Vector3* values, int count)
{
	static_assert(sizeof(Vector3) == 3 * sizeof(GLfloat), "Vector3 seems to have additional fields");
	// All values consecutively in memory: pass pointer to first value
	gl.Uniform3fv(GetUniformLocation(key), count, values[0]);
}

inline void GlCommandSink::Program::SetUniform(Hash key, const Vector4* values, int count)
{
	static_assert(sizeof(Vector4) == 4 * sizeof(GLfloat), "Vector4 seems to have additional fields");
	// All values consecutively in memory: pass pointer to first value
	gl.Uniform4fv(GetUniformLocation(key), count, values[0]);
}

inline void GlCommandSink::Program::SetUniform(Hash key, const IntVector2* values, int count)
{
	static_assert(sizeof(IntVector2) == 2 * sizeof(GLint), "IntVector2 seems to have additional fields");
	// All values consecutively in memory: pass pointer to first value
	gl.Uniform2iv(GetUniformLocation(key), count, values[0]);
}

inline void GlCommandSink::Program::SetUniform(Hash key, const IntVector3* values, int count)
{
	static_assert(sizeof(IntVector3) == 3 * sizeof(GLint), "IntVector3 seems to have additional fields");
	// All values consecutively in memory: pass pointer to first value
	gl.Uniform3iv(GetUniformLocation(key), count, values[0]);
}

inline void GlCommandSink::Program::SetUniform(Hash key, const IntVector4* values, int count)
{
	static_assert(sizeof(IntVector4) == 4 * sizeof(GLint), "IntVector4 seems to have additional fields");
	// All values consecutively in memory: pass pointer to first value
	gl.Uniform4iv(GetUniformLocation(key), count, values[0]);
}

inline void GlCommandSink::Program::SetUniform(Hash key, const UIntVector2 values[], int count)
{
	static_assert(sizeof(UIntVector2) == 2 * sizeof(GLuint), "UIntVector4 seems to have additional fields");
	// All values consecutively in memory: pass pointer to first value
	gl.Uniform2uiv(GetUniformLocation(key), count, values[0]);
}

inline void GlCommandSink::Program::SetUniform(Hash key, const UIntVector3 values[], int count)
{
	static_assert(sizeof(UIntVector3) == 3 * sizeof(GLuint), "UIntVector4 seems to have additional fields");
	// All values consecutively in memory: pass pointer to first value
	gl.Uniform3uiv(GetUniformLocation(key), count, values[0]);
}

inline void GlCommandSink::Program::SetUniform(Hash key, const UIntVector4 values[], int count)
{
	static_assert(sizeof(UIntVector4) == 4 * sizeof(GLuint), "UIntVector4 seems to have additional fields");
	// All values consecutively in memory: pass pointer to first value
	gl.Uniform4uiv(GetUniformLocation(key), count, values[0]);
}

inline void GlCommandSink::Program::SetUniform(Hash key, const Matrix<4,4>* values, int count)
{
	static_assert(sizeof(Matrix<4,4>) == 4 * 4 * sizeof(GLfloat), "Matrix<4,4> seems to have additional fields");
	// All values consecutively in memory: pass pointer to first value
	gl.UniformMatrix4fv(GetUniformLocation(key), count, GL_TRUE, values[0].Get());
	// The transpose argument is ignored for OpelGL ES versions prior to 3.0
}

inline void GlCommandSink::Program::SetUniform(Hash key, const Matrix<3,3>* values, int count)
{
	static_assert(sizeof(Matrix<3,3>) == 3 * 3 * sizeof(GLfloat), "Matrix<3,3> seems to have additional fields");
	// All values consecutively in memory: pass pointer to first value
	gl.UniformMatrix3fv(GetUniformLocation(key), count, GL_TRUE, values[0].Get());
	// The transpose argument is ignored for OpelGL ES versions prior to 3.0
}

inline GLint GlCommandSink::Program::GetUniformLocation(Hash hash) const
{
	auto it = mUniformLocations.find(hash);
	if(it == mUniformLocations.end())
		return -1;
	else
		return it->second;
}

}
}

#endif // MOLECULAR_GLCOMMANDSINKPROGRAM_H
