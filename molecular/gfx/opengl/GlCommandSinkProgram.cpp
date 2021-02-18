/*	GlCommandSinkProgram.cpp

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

#include "GlCommandSinkProgram.h"
#include <cassert>

namespace molecular
{
namespace gfx
{

void GlCommandSink::Program::Store(const std::string& vertexShaderSource, const std::string& geometryShaderSource, const std::string& pixelShaderSource)
{
	if(geometryShaderSource.empty())
	{
		ShaderSourceItem items[2] =
		{
			{kVertexShader, vertexShaderSource.data(), vertexShaderSource.size()},
			{kFragmentShader, pixelShaderSource.data(), pixelShaderSource.size()}
		};
		Store(items, 2);
	}
	else {
		ShaderSourceItem items[3] =
		{
			{kVertexShader, vertexShaderSource.data(), vertexShaderSource.size()},
			{kGeometryShader, geometryShaderSource.data(), geometryShaderSource.size()},
			{kFragmentShader, pixelShaderSource.data(), pixelShaderSource.size()}
		};
		Store(items, 3);
	}
}

void GlCommandSink::Program::Store(const ShaderSourceItem items[], size_t count, const char** transformFeedbackOutputs,  unsigned int tfOutputsCount)
{
	mUniformLocations.clear();
	mInputs.clear();
	mTextureUnits.clear();
	mAttributes.clear();
	// TODO: glDisableVertexAttribArray (Non-ES only)

	std::vector<Shader> shaders;

	for(size_t i = 0; i < count; ++i)
	{
		shaders.emplace_back(items[i].type);
		Shader& shader = shaders.back();
		shader.SourceCompile(items[i].source, items[i].size);
		gl.AttachShader(mProgram, shader.mShader);
        CheckError("glAttachShader", __LINE__, __FILE__);
	}

	if(gl.HasBindFragDataLocation())
	{
		gl.BindFragDataLocation(mProgram, 0, "fragmentColor");
		CheckError("glBindFragDataLocation", __LINE__, __FILE__);
	}

	if(transformFeedbackOutputs)
	{
		gl.TransformFeedbackVaryings(mProgram, tfOutputsCount, transformFeedbackOutputs, gl.INTERLEAVED_ATTRIBS);
	}

	gl.LinkProgram(mProgram);
    CheckError("glLinkProgram", __LINE__, __FILE__);

	for(auto& shader: shaders)
	{
		gl.DetachShader(mProgram, shader.mShader);
	}

	Postprocess();
}

void GlCommandSink::Program::SetUniform(Hash key, const Texture* const* textures, int count)
{
	if(!textures)
		return;

	unsigned int unit = 0;
	// Find texture unit:
	for(; unit < mTextureUnits.size(); ++unit)
	{
		if(mTextureUnits[unit] == key)
			break;
	}

	for(size_t i = 0; i < static_cast<size_t>(count) && i < mTextureUnits.size() - unit; ++i)
	{
		if(textures[i] && mTextureUnits[unit + i] == key)
		{
			gl.ActiveTexture(gl.TEXTURE0 + unit + i);
            CheckError("glActiveTexture", __LINE__, __FILE__);
			if(textures[i]->GetDepth() > 0)
				gl.BindTexture(gl.TEXTURE_3D, textures[i]->mTexture);
			else
				gl.BindTexture(GL_TEXTURE_2D, textures[i]->mTexture);
			CheckError("glBindTexture", __LINE__, __FILE__);

		}
	}
}

void GlCommandSink::Program::SetAttribute(Hash key, VertexBuffer* buffer, int components, VertexAttributeInfo::Type type, int stride, int offset, bool normalized)
{
	assert(buffer);

	// Find index:
	for(unsigned int index = 0; index < mAttributes.size(); index++)
	{
		if(mAttributes[index] == key)
		{
			gl.BindBuffer(gl.ARRAY_BUFFER, buffer->mBuffer);
            CheckError("glBindBuffer", __LINE__, __FILE__);

			const GLvoid* glOffset = reinterpret_cast<const GLvoid*>(offset);

			if(mAttributeIsInts[index])
				gl.VertexAttribIPointer(index, components, ToGlEnum(type), stride, glOffset);
			else
				gl.VertexAttribPointer(index, components, ToGlEnum(type), normalized ? GL_TRUE : GL_FALSE, stride, glOffset);

            CheckError("glVertexAttribIPointer", __LINE__, __FILE__);
			return;
		}
	}
	LOG(WARNING) << "Attribute not found";
}

GlCommandSink::Program::Program()
{
	mProgram = gl.CreateProgram();
    CheckError("glCreateProgram", __LINE__, __FILE__);
	gl.GenVertexArrays(1, &mVertexArrayObject);
    CheckError("glGenVertexArrays", __LINE__, __FILE__);
}

GlCommandSink::Program::~Program()
{
	gl.DeleteVertexArrays(1, &mVertexArrayObject);
    CheckError("glDeleteVertexArrays", __LINE__, __FILE__);
	gl.DeleteProgram(mProgram);
    CheckError("glDeleteProgram", __LINE__, __FILE__);
}

GlCommandSink::Program::Shader::Shader(ShaderType type)
{
	mShader = gl.CreateShader(type);
	if(!mShader)
	{
		GLenum error = glGetError();
		throw std::runtime_error(std::string("glCreateShader: ") + GlConstantString(error));
	}
}

bool GlCommandSink::Program::Shader::SourceCompile(const char* text, size_t length, bool reportError)
{
	/* Generated code is compatible with GLSL versions "150" and "300 es". */
	const char* versionString = "";
	if(glslVersion == GlslVersion::V_150)
		versionString = "#version 150\n";
	else if(glslVersion == GlslVersion::V_300_ES)
		versionString = "#version 300 es\nprecision highp float;\n"; // TODO: selective precision
	else if(glslVersion == GlslVersion::V_330)
		versionString = "#version 330\n";

	const char* strings[] = {versionString, text};
	GLint lengths[] = {static_cast<GLint>(strlen(versionString)), static_cast<GLint>(length)};
	gl.ShaderSource(mShader, 2, strings, lengths);
	CheckError("glShaderSource", __LINE__, __FILE__);
	gl.CompileShader(mShader);
	CheckError("glCompileShader", __LINE__, __FILE__);
	GLint compileStatus = 0;
	gl.GetShaderiv(mShader, gl.COMPILE_STATUS, &compileStatus);
	CheckError("glGetShaderiv", __LINE__, __FILE__);
	if(compileStatus != GL_TRUE)
	{
		char error[1024];
		GLsizei outLength = 0;
		gl.GetShaderInfoLog(mShader, 1024, &outLength, error);
		if(outLength > 0 && reportError)
		{
			LOG(ERROR) << std::string(error, outLength);
			LOG(DEBUG) << text;
		}
		return false;
	}
	return true;
}

void GlCommandSink::Program::Postprocess()
{
	gl.UseProgram(mProgram);
	gl.BindVertexArray(mVertexArrayObject);
    CheckError("glBindVertexArray", __LINE__, __FILE__);

	// Get all uniform names and assign texture units if applicable:
	GLint numUniforms = 0;
	gl.GetProgramiv(mProgram, gl.ACTIVE_UNIFORMS, &numUniforms);
	for(GLint i = 0; i < numUniforms; ++i)
	{
		char name[64];
		GLsizei nameLength;
		GLint size;
		GLenum type;
		// Get uniform name:
		gl.GetActiveUniform(mProgram, i, 64, &nameLength, &size, &type, name);
        CheckError("glGetActiveUniform", __LINE__, __FILE__);

		if(nameLength >= 3 && !strncmp(name, "gl_", 3))
			continue; // Skip builtins

		if(nameLength >= 1 && name[nameLength - 1] == ']')
		{
			// strip array subscripts
			while(nameLength > 0 && name[nameLength-1] != '[')
				nameLength--;

			nameLength--;
			name[nameLength] = 0;
		}

		Hash hash = HashUtils::MakeHash(name, nameLength);
		GLint location = gl.GetUniformLocation(mProgram, name);
		mUniformLocations[hash] = location;
		mInputs.insert(hash);

		// Pre-assign texture unit IDs
		if(IsSamplerType(type))
		{
			assert(size <= 48); // Constant number for simplicity
			GLint textureUnitNumbers[48];

			for(GLint i = 0; i < size; ++i)
			{
//				std::cerr << "GL_TEXTURE" << mTextureUnits.size() << ": " << name << std::endl;
				textureUnitNumbers[i] = mTextureUnits.size();
				mTextureUnits.push_back(hash);
			}
			gl.Uniform1iv(location, size, textureUnitNumbers);
            CheckError("glUniform1iv", __LINE__, __FILE__);
		}
	}

	// Get all attribute names from program and activate attributes in VAO:
	GLint numAttributes = 0;
	gl.GetProgramiv(mProgram, gl.ACTIVE_ATTRIBUTES, &numAttributes);
    CheckError("glGetProgramiv", __LINE__, __FILE__);
    numAttributes += 1; // TODO: Hack only. Attribute locations can be all over the place?!
    mAttributes.resize(numAttributes);
    mAttributeIsInts.resize(numAttributes);

    for(GLint i = 0; i < numAttributes - 1; ++i)
	{
		char name[64];
		GLsizei nameLength;
		GLint size;
		GLenum type;
		gl.GetActiveAttrib(mProgram, i, 64, &nameLength, &size, &type, name);
        CheckError("glGetActiveAttrib", __LINE__, __FILE__);
		Hash hash = HashUtils::MakeHash(name, nameLength);
		mInputs.insert(hash);
		// TODO: Attribute arrays?
		assert(size == 1);

		// location is not necessarily equal to i!
		GLint location = gl.GetAttribLocation(mProgram, name);
        CheckError("glGetAttribLocation", __LINE__, __FILE__);
        assert(location >= 0);
		assert(location < numAttributes);
		mAttributes.at(location) = hash;
		mAttributeIsInts.at(location) = IsIntType(type);

		/* This is saved in the vertex array object. TODO for ES2: Call in
			SetAttribute(). And disable somewhere! */
		gl.EnableVertexAttribArray(location);
        CheckError("glEnableVertexAttribArray", __LINE__, __FILE__);

		// glBindAttribLocation has to be called before glLinkProgram:
/*		glBindAttribLocation(mProgram, index, name);
		CheckError("glBindAttribLocation", __LINE__); */
	}
}

bool GlCommandSink::Program::IsSamplerType(GLenum uniformType)
{
	switch(uniformType)
	{
	case gl.SAMPLER_1D:
	case gl.SAMPLER_1D_SHADOW:
	case gl.SAMPLER_1D_ARRAY:
	case gl.SAMPLER_1D_ARRAY_SHADOW:
	case gl.SAMPLER_2D_MULTISAMPLE:
	case gl.SAMPLER_2D_MULTISAMPLE_ARRAY:
	case gl.SAMPLER_2D_RECT:
	case gl.SAMPLER_2D_RECT_SHADOW:
	case gl.INT_SAMPLER_1D:
	case gl.INT_SAMPLER_1D_ARRAY:
	case gl.UNSIGNED_INT_SAMPLER_1D:
	case gl.UNSIGNED_INT_SAMPLER_1D_ARRAY:
	case gl.SAMPLER_2D:
	case gl.SAMPLER_3D:
	case gl.SAMPLER_CUBE:
	case gl.SAMPLER_2D_SHADOW:
	case gl.SAMPLER_2D_ARRAY:
	case gl.SAMPLER_2D_ARRAY_SHADOW:
	case gl.SAMPLER_CUBE_SHADOW:
//	case gl.SAMPLER_BUFFER:
	case gl.INT_SAMPLER_2D:
	case gl.INT_SAMPLER_3D:
	case gl.INT_SAMPLER_CUBE:
	case gl.INT_SAMPLER_2D_ARRAY:
	case gl.UNSIGNED_INT_SAMPLER_2D:
	case gl.UNSIGNED_INT_SAMPLER_3D:
	case gl.UNSIGNED_INT_SAMPLER_CUBE:
	case gl.UNSIGNED_INT_SAMPLER_2D_ARRAY:
		return true;
	default:
		return false;
	}
}

bool GlCommandSink::Program::IsIntType(GLenum type)
{
	switch(type)
	{
	case gl.INT:
	case gl.INT_VEC2:
	case gl.INT_VEC3:
	case gl.INT_VEC4:
	case gl.UNSIGNED_INT:
	case gl.UNSIGNED_INT_VEC2:
	case gl.UNSIGNED_INT_VEC3:
	case gl.UNSIGNED_INT_VEC4:
		return true;
	default:
		return false;
	}
}

}
}
