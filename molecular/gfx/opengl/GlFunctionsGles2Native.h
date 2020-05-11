/*	GlFunctionsGles2Native.h

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

#ifndef MOLECULAR_GLFUNCTIONSGLES2NATIVE_H
#define MOLECULAR_GLFUNCTIONSGLES2NATIVE_H

#include "OpenGlPrerequisites.h"
#include "GlFunctionsGl10Native.h"

namespace molecular
{
namespace gfx
{

/// Native OpenGL calls up to OpenGL ES 2.0
/** Used on Apple. Also works in conjunction with GLEW or GL3W. */
class GlFunctionsGles2Native : public GlFunctionsGl10NativeNoGles2
{
public:
	void ActiveTexture(GLenum texture) {glActiveTexture(texture);}
	void AttachShader(GLuint program, GLuint shader) {glAttachShader(program, shader);}
	void BindBuffer(GLenum target, GLuint buffer) {glBindBuffer(target, buffer);}
	void BindFramebuffer(GLenum target, GLuint framebuffer) {glBindFramebuffer(target, framebuffer);}
	void BindRenderbuffer(GLenum target, GLuint renderbuffer) {glBindRenderbuffer(target, renderbuffer);}
	void BindTexture(GLenum target, GLuint texture) {glBindTexture(target, texture);}
	GLenum CheckFramebufferStatus(GLenum target) {return glCheckFramebufferStatus(target);}
	void CompileShader(GLuint shader) {glCompileShader(shader);}
	void BufferData(GLenum target, GLsizeiptr size, const GLvoid* data, GLenum usage) {glBufferData(target, size, data, usage);}
	void CompressedTexImage2D(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const GLvoid* data) {glCompressedTexImage2D(target, level, internalformat, width, height, border, imageSize, data);}
	GLuint CreateProgram() {return glCreateProgram();}
	GLuint CreateShader(GLenum type) {return glCreateShader(type);}
	void CompressedTexSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const GLvoid* data) {glCompressedTexSubImage2D(target, level, xoffset, yoffset, width, height, format, imageSize, data);}
	void DeleteBuffers(GLsizei n, const GLuint * buffers) { glDeleteBuffers(n, buffers); }
	void DeleteFramebuffers(GLsizei n, const GLuint* framebuffers) {glDeleteFramebuffers(n, framebuffers);}
	void DeleteProgram(GLuint program) {glDeleteProgram(program);}
	void DeleteRenderbuffers(GLsizei n, const GLuint* renderbuffers) {glDeleteRenderbuffers(n, renderbuffers);}
	void DeleteShader(GLuint shader) { glDeleteShader(shader); }
	void DetachShader(GLuint program, GLuint shader) {glDetachShader(program, shader);}
	void DisableVertexAttribArray(GLuint index) { glDisableVertexAttribArray(index); }
	void DrawArrays(GLenum mode, GLint first, GLsizei count) {glDrawArrays(mode, first, count);}
	void DrawElements(GLenum mode, GLsizei count, GLenum type, const GLvoid* indices) {glDrawElements(mode, count, type, indices);}
	void EnableVertexAttribArray(GLuint index) {glEnableVertexAttribArray(index);}
	void FramebufferRenderbuffer(GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer) {glFramebufferRenderbuffer(target, attachment, renderbuffertarget, renderbuffer);}
	void FramebufferTexture2D(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level) {glFramebufferTexture2D(target, attachment, textarget, texture, level);}
	void GenBuffers(GLsizei n, GLuint* buffers) {glGenBuffers(n, buffers);}
	void GenerateMipmap(GLenum target) { glGenerateMipmap(target); }
	void GenFramebuffers(GLsizei n, GLuint* framebuffers) {glGenFramebuffers(n, framebuffers);}
	void GenRenderbuffers(GLsizei n, GLuint* renderbuffers) {glGenRenderbuffers(n, renderbuffers);}
	void GetActiveAttrib(GLuint program, GLuint index, GLsizei bufSize, GLsizei* length, GLint* size, GLenum* type, GLchar* name) {glGetActiveAttrib(program, index, bufSize, length, size, type, name);}
	void GetActiveUniform(GLuint program, GLuint index, GLsizei bufSize, GLsizei* length, GLint* size, GLenum* type, GLchar* name) {glGetActiveUniform(program, index, bufSize, length, size, type, name);}
	GLint GetAttribLocation(GLuint program, const GLchar* name) {return glGetAttribLocation(program, name);}
	void GetProgramiv(GLuint program, GLenum pname, GLint* params) {glGetProgramiv(program, pname, params);}
	void GetShaderInfoLog(GLuint shader, GLsizei bufSize, GLsizei* length, GLchar* infoLog) {glGetShaderInfoLog(shader, bufSize, length, infoLog);}
	void GetShaderiv(GLuint shader, GLenum pname, GLint* params) {glGetShaderiv(shader, pname, params);}
	GLint GetUniformLocation(GLuint program, const GLchar* name) {return glGetUniformLocation(program, name);}
	void LinkProgram(GLuint program) {glLinkProgram(program);}
	void RenderbufferStorage(GLenum target, GLenum internalformat, GLsizei width, GLsizei height) {glRenderbufferStorage(target, internalformat, width, height);}
	void ShaderSource(GLuint shader, GLsizei count, const GLchar* const* string, const GLint* length) {glShaderSource(shader, count, string, length);}
	void TexSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid* pixels) {glTexSubImage2D(target, level, xoffset, yoffset, width, height, format, type, pixels);}
	void Uniform1fv(GLint location, GLsizei count, const GLfloat* value) {glUniform1fv(location, count, value);}
	void Uniform2fv(GLint location, GLsizei count, const GLfloat* value) {glUniform2fv(location, count, value);}
	void Uniform3fv(GLint location, GLsizei count, const GLfloat* value) {glUniform3fv(location, count, value);}
	void Uniform4fv(GLint location, GLsizei count, const GLfloat* value) {glUniform4fv(location, count, value);}
	void Uniform1iv(GLint location, GLsizei count, const GLint* value) {glUniform1iv(location, count, value);}
	void Uniform2iv(GLint location, GLsizei count, const GLint* value) {glUniform2iv(location, count, value);}
	void Uniform3iv(GLint location, GLsizei count, const GLint* value) {glUniform3iv(location, count, value);}
	void Uniform4iv(GLint location, GLsizei count, const GLint* value) {glUniform4iv(location, count, value);}
	void UniformMatrix3fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value) {glUniformMatrix3fv(location, count, transpose, value);}
	void UniformMatrix4fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value) {glUniformMatrix4fv(location, count, transpose, value);}
	void UseProgram(GLuint program) {glUseProgram(program);}
	void VertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid* pointer) {glVertexAttribPointer(index, size, type, normalized, stride, pointer);}
	void ValidateProgram(GLuint program) {glValidateProgram(program);}
};

}
}

#endif // GLFUNCTIONSGLES2NATIVE_H

