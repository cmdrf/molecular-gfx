/*	GlFunctionsGles2ProcAddresses.h
	Copyright 2015-2016 Fabian Herb

	This file is part of Molecular Engine.
*/

#ifndef GLFUNCTIONSGLES2PROCADDRESSES_H
#define GLFUNCTIONSGLES2PROCADDRESSES_H

#include "OpenGlPrerequisites.h"
#include <cstddef> // ptrdiff_t

/// Procedure Addresses for functions that are in GLES 2.0, but not in OpenGL 1.0
/** Required on Windows, where everything except OpenGL 1.0 must be acquired as
	procedure addresses. */
template<class TInitializer>
class GlFunctionsGles2ProcAddresses
{
public:
	typedef ptrdiff_t GLsizeiptr;
	typedef char GLchar;

	void Init();

	void ActiveTexture(GLenum texture) {mActiveTexture(texture);}
	void AttachShader(GLuint program, GLuint shader) {mAttachShader(program, shader);}
	void BindBuffer(GLenum target, GLuint buffer) {mBindBuffer(target, buffer);}
	void BindFramebuffer(GLenum target, GLuint framebuffer) {mBindFramebuffer(target, framebuffer);}
	void BindRenderbuffer(GLenum target, GLuint renderbuffer) {mBindRenderbuffer(target, renderbuffer);}
	void BindTexture(GLenum target, GLuint texture) {mBindTexture(target, texture);}
	GLenum CheckFramebufferStatus(GLenum target) {return mCheckFramebufferStatus(target);}
	void CompileShader(GLuint shader) {mCompileShader(shader);}
	void BufferData(GLenum target, GLsizeiptr size, const GLvoid* data, GLenum usage) {mBufferData(target, size, data, usage);}
	void CompressedTexImage2D(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const GLvoid* data) {mCompressedTexImage2D(target, level, internalformat, width, height, border, imageSize, data);}
	GLuint CreateProgram() {return mCreateProgram();}
	GLuint CreateShader(GLenum type) {return mCreateShader(type);}
	void CompressedTexSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const GLvoid* data) {mCompressedTexSubImage2D(target, level, xoffset, yoffset, width, height, format, imageSize, data);}
	void DeleteBuffers(GLsizei n, const GLuint * buffers) { mDeleteBuffers(n, buffers); }
	void DeleteFramebuffers(GLsizei n, const GLuint* framebuffers) {mDeleteFramebuffers(n, framebuffers);}
	void DeleteProgram(GLuint program) {mDeleteProgram(program);}
	void DeleteRenderbuffers(GLsizei n, const GLuint* renderbuffers) {mDeleteRenderbuffers(n, renderbuffers);}
	void DeleteShader(GLuint shader) { mDeleteShader(shader); }
	void DetachShader(GLuint program, GLuint shader) {mDetachShader(program, shader);}
	void DisableVertexAttribArray(GLuint index) { mDisableVertexAttribArray(index); }
	void DrawArrays(GLenum mode, GLint first, GLsizei count) {mDrawArrays(mode, first, count);}
	void DrawElements(GLenum mode, GLsizei count, GLenum type, const GLvoid* indices) {mDrawElements(mode, count, type, indices);}
	void EnableVertexAttribArray(GLuint index) {mEnableVertexAttribArray(index);}
	void FramebufferRenderbuffer(GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer) {mFramebufferRenderbuffer(target, attachment, renderbuffertarget, renderbuffer);}
	void FramebufferTexture2D(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level) {mFramebufferTexture2D(target, attachment, textarget, texture, level);}
	void GenBuffers(GLsizei n, GLuint* buffers) {mGenBuffers(n, buffers);}
	void GenerateMipmap(GLenum target) { mGenerateMipmap(target); }
	void GenFramebuffers(GLsizei n, GLuint* framebuffers) {mGenFramebuffers(n, framebuffers);}
	void GenRenderbuffers(GLsizei n, GLuint* renderbuffers) {mGenRenderbuffers(n, renderbuffers);}
	void GetActiveAttrib(GLuint program, GLuint index, GLsizei bufSize, GLsizei* length, GLint* size, GLenum* type, GLchar* name) {mGetActiveAttrib(program, index, bufSize, length, size, type, name);}
	void GetActiveUniform(GLuint program, GLuint index, GLsizei bufSize, GLsizei* length, GLint* size, GLenum* type, GLchar* name) {mGetActiveUniform(program, index, bufSize, length, size, type, name);}
	GLint GetAttribLocation(GLuint program, const GLchar* name) {return mGetAttribLocation(program, name);}
	void GetProgramiv(GLuint program, GLenum pname, GLint* params) {mGetProgramiv(program, pname, params);}
	void GetShaderInfoLog(GLuint shader, GLsizei bufSize, GLsizei* length, GLchar* infoLog) {mGetShaderInfoLog(shader, bufSize, length, infoLog);}
	void GetShaderiv(GLuint shader, GLenum pname, GLint* params) {mGetShaderiv(shader, pname, params);}
	GLint GetUniformLocation(GLuint program, const GLchar* name) {return mGetUniformLocation(program, name);}
	void LinkProgram(GLuint program) {mLinkProgram(program);}
	void RenderbufferStorage(GLenum target, GLenum internalformat, GLsizei width, GLsizei height) {mRenderbufferStorage(target, internalformat, width, height);}
	void ShaderSource(GLuint shader, GLsizei count, const GLchar* const* string, const GLint* length) {mShaderSource(shader, count, string, length);}
	void TexSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid* pixels) {mTexSubImage2D(target, level, xoffset, yoffset, width, height, format, type, pixels);}
	void Uniform1fv(GLint location, GLsizei count, const GLfloat* value) {mUniform1fv(location, count, value);}
	void Uniform2fv(GLint location, GLsizei count, const GLfloat* value) {mUniform2fv(location, count, value);}
	void Uniform3fv(GLint location, GLsizei count, const GLfloat* value) {mUniform3fv(location, count, value);}
	void Uniform4fv(GLint location, GLsizei count, const GLfloat* value) {mUniform4fv(location, count, value);}
	void Uniform1iv(GLint location, GLsizei count, const GLint* value) {mUniform1iv(location, count, value);}
	void Uniform2iv(GLint location, GLsizei count, const GLint* value) {mUniform2iv(location, count, value);}
	void Uniform3iv(GLint location, GLsizei count, const GLint* value) {mUniform3iv(location, count, value);}
	void Uniform4iv(GLint location, GLsizei count, const GLint* value) {mUniform4iv(location, count, value);}
	void UniformMatrix3fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value) {mUniformMatrix3fv(location, count, transpose, value);}
	void UniformMatrix4fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value) {mUniformMatrix4fv(location, count, transpose, value);}
	void UseProgram(GLuint program) {mUseProgram(program);}
	void VertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid* pointer) {mVertexAttribPointer(index, size, type, normalized, stride, pointer);}
	void ValidateProgram(GLuint program) {mValidateProgram(program);}

private:
	using ActiveTextureType = void (*)(GLenum texture);
	using AttachShaderType = void (*)(GLuint program, GLuint shader);
	using BindBufferType = void (*)(GLenum target, GLuint buffer);
	using BindFramebufferType = void (*)(GLenum target, GLuint framebuffer);
	using BindRenderbufferType = void (*)(GLenum target, GLuint renderbuffer);
	using BindTextureType = void (*)(GLenum target, GLuint texture);
	using CheckFramebufferStatusType = GLenum (*)(GLenum target);
	using CompileShaderType = void (*)(GLuint shader);
	using BufferDataType = void (*)(GLenum target, GLsizeiptr size, const GLvoid* data, GLenum usage);
	using CompressedTexImage2DType = void (*)(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const GLvoid* data);
	using CreateProgramType = GLuint (*)();
	using CreateShaderType = GLuint (*)(GLenum type);
	using CompressedTexSubImage2DType = void (*)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const GLvoid* data);
	using DeleteBuffersType = void(*)(GLsizei n, const GLuint * buffers);
	using DeleteFramebuffersType = void (*)(GLsizei n, const GLuint* framebuffers);
	using DeleteProgramType = void (*)(GLuint program);
	using DeleteRenderbuffersType = void (*)(GLsizei n, const GLuint* renderbuffers);
	using DeleteShaderType = void (*)(GLuint shader);
	using DetachShaderType = void (*)(GLuint program, GLuint shader);
	using DisableVertexAttribArrayType = void(*)(GLuint index);
	using DrawArraysType = void (*)(GLenum mode, GLint first, GLsizei count);
	using DrawElementsType = void (*)(GLenum mode, GLsizei count, GLenum type, const GLvoid* indices);
	using EnableVertexAttribArrayType = void (*)(GLuint index);
	using FramebufferRenderbufferType = void (*)(GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer);
	using FramebufferTexture2DType = void (*)(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
	using GenBuffersType = void (*)(GLsizei n, GLuint* buffers);
	using GenerateMipmapType = void(*)(GLenum target);
	using GenFramebuffersType = void (*)(GLsizei n, GLuint* framebuffers);
	using GenRenderbuffersType = void (*)(GLsizei n, GLuint* renderbuffers);
	using GetActiveAttribType = void (*)(GLuint program, GLuint index, GLsizei bufSize, GLsizei* length, GLint* size, GLenum* type, GLchar* name);
	using GetActiveUniformType = void (*)(GLuint program, GLuint index, GLsizei bufSize, GLsizei* length, GLint* size, GLenum* type, GLchar* name);
	using GetAttribLocationType = GLint (*)(GLuint program, const GLchar* name);
	using GetProgramivType = void (*)(GLuint program, GLenum pname, GLint* params);
	using GetShaderInfoLogType = void (*)(GLuint shader, GLsizei bufSize, GLsizei* length, GLchar* infoLog);
	using GetShaderivType = void (*)(GLuint shader, GLenum pname, GLint* params);
	using GetUniformLocationType = GLint (*)(GLuint program, const GLchar* name);
	using LinkProgramType = void (*)(GLuint program);
	using RenderbufferStorageType = void (*)(GLenum target, GLenum internalformat, GLsizei width, GLsizei height);
	using ShaderSourceType = void (*)(GLuint shader, GLsizei count, const GLchar* const* string, const GLint* length);
	using TexSubImage2DType = void (*)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid* pixels);
	using Uniform1fvType = void (*)(GLint location, GLsizei count, const GLfloat* value);
	using Uniform2fvType = void (*)(GLint location, GLsizei count, const GLfloat* value);
	using Uniform3fvType = void (*)(GLint location, GLsizei count, const GLfloat* value);
	using Uniform4fvType = void (*)(GLint location, GLsizei count, const GLfloat* value);
	using Uniform1ivType = void (*)(GLint location, GLsizei count, const GLint* value);
	using Uniform2ivType = void (*)(GLint location, GLsizei count, const GLint* value);
	using Uniform3ivType = void (*)(GLint location, GLsizei count, const GLint* value);
	using Uniform4ivType = void (*)(GLint location, GLsizei count, const GLint* value);
	using UniformMatrix3fvType = void (*)(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
	using UniformMatrix4fvType = void (*)(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
	using UseProgramType = void (*)(GLuint program);
	using VertexAttribPointerType = void (*)(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid* pointer);
	using ValidateProgramType = void (*)(GLuint program);

	ActiveTextureType mActiveTexture = nullptr;
	AttachShaderType mAttachShader = nullptr;
	BindBufferType mBindBuffer = nullptr;
	BindFramebufferType mBindFramebuffer = nullptr;
	BindRenderbufferType mBindRenderbuffer = nullptr;
	BindTextureType mBindTexture = nullptr;
	CheckFramebufferStatusType mCheckFramebufferStatus = nullptr;
	CompileShaderType mCompileShader = nullptr;
	BufferDataType mBufferData = nullptr;
	CompressedTexImage2DType mCompressedTexImage2D = nullptr;
	CreateProgramType mCreateProgram = nullptr;
	CreateShaderType mCreateShader = nullptr;
	CompressedTexSubImage2DType mCompressedTexSubImage2D = nullptr;
	DeleteBuffersType mDeleteBuffers = nullptr;
	DeleteFramebuffersType mDeleteFramebuffers = nullptr;
	DeleteProgramType mDeleteProgram = nullptr;
	DeleteRenderbuffersType mDeleteRenderbuffers = nullptr;
	DeleteShaderType mDeleteShader = nullptr;
	DetachShaderType mDetachShader = nullptr;
	DisableVertexAttribArrayType mDisableVertexAttribArray = nullptr;
	DrawArraysType mDrawArrays = nullptr;
	DrawElementsType mDrawElements = nullptr;
	EnableVertexAttribArrayType mEnableVertexAttribArray = nullptr;
	FramebufferRenderbufferType mFramebufferRenderbuffer = nullptr;
	FramebufferTexture2DType mFramebufferTexture2D = nullptr;
	GenBuffersType mGenBuffers = nullptr;
	GenerateMipmapType mGenerateMipmap = nullptr;
	GenFramebuffersType mGenFramebuffers = nullptr;
	GenRenderbuffersType mGenRenderbuffers = nullptr;
	GetActiveAttribType mGetActiveAttrib = nullptr;
	GetActiveUniformType mGetActiveUniform = nullptr;
	GetAttribLocationType mGetAttribLocation = nullptr;
	GetProgramivType mGetProgramiv = nullptr;
	GetShaderInfoLogType mGetShaderInfoLog = nullptr;
	GetShaderivType mGetShaderiv = nullptr;
	GetUniformLocationType mGetUniformLocation = nullptr;
	LinkProgramType mLinkProgram = nullptr;
	RenderbufferStorageType mRenderbufferStorage = nullptr;
	ShaderSourceType mShaderSource = nullptr;
	TexSubImage2DType mTexSubImage2D = nullptr;
	Uniform1fvType mUniform1fv = nullptr;
	Uniform2fvType mUniform2fv = nullptr;
	Uniform3fvType mUniform3fv = nullptr;
	Uniform4fvType mUniform4fv = nullptr;
	Uniform1ivType mUniform1iv = nullptr;
	Uniform2ivType mUniform2iv = nullptr;
	Uniform3ivType mUniform3iv = nullptr;
	Uniform4ivType mUniform4iv = nullptr;
	UniformMatrix3fvType mUniformMatrix3fv = nullptr;
	UniformMatrix4fvType mUniformMatrix4fv = nullptr;
	UseProgramType mUseProgram = nullptr;
	VertexAttribPointerType mVertexAttribPointer = nullptr;
	ValidateProgramType mValidateProgram = nullptr;
};

/// Procedure addresses for OpenGL 1.0
/** unused */
template<class TInitializer>
class OpenGL10ProcAddresses
{
public:
	void Init();

	void Clear(GLbitfield mask) {mClear(mask);}
	void Enable(GLenum cap) {mEnable(cap);}
	void GetIntegerv(GLenum pname, GLint* params) {mGetIntegerv(pname, params);}
	void ReadPixels(GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLvoid* pixels) {mReadPixels(x, y, width, height, format, type, pixels);}
	void TexImage2D(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid* pixels) {mTexImage2D(target, level, internalformat, width, height, border, format, type, pixels);}
	void TexParameteri(GLenum target, GLenum pname, GLint param) {mTexParameteri(target, pname, param);}
	void Viewport(GLint x, GLint y, GLsizei width, GLsizei height) {mViewport(x, y, width, height);}

private:
	using ClearType = void (*)(GLbitfield mask);
	using EnableType = void (*)(GLenum cap);
	using GetIntegervType = void (*)(GLenum pname, GLint* params);
	using ReadPixelsType = void (*)(GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLvoid* pixels);
	using TexImage2DType = void (*)(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid* pixels);
	using TexParameteriType = void (*)(GLenum target, GLenum pname, GLint param);
	using ViewportType = void (*)(GLint x, GLint y, GLsizei width, GLsizei height);

	ClearType mClear = nullptr;
	EnableType mEnable = nullptr;
	GetIntegervType mGetIntegerv = nullptr;
	ReadPixelsType mReadPixels = nullptr;
	TexImage2DType mTexImage2D = nullptr;
	TexParameteriType mTexParameteri = nullptr;
	ViewportType mViewport = nullptr;

};

template<class TInitializer>
void GlFunctionsGles2ProcAddresses<TInitializer>::Init()
{
	using I = TInitializer;
	I::Init(mActiveTexture, "glActiveTexture");
	I::Init(mAttachShader, "glAttachShader");
	I::Init(mBindBuffer, "glBindBuffer");
	I::Init(mBindFramebuffer, "glBindFramebuffer");
	I::Init(mBindRenderbuffer, "glBindRenderbuffer");
	I::Init(mBindTexture, "glBindTexture");
	I::Init(mCheckFramebufferStatus, "glCheckFramebufferStatus");
	I::Init(mCompileShader, "glCompileShader");
	I::Init(mBufferData, "glBufferData");
	I::Init(mCompressedTexImage2D, "glCompressedTexImage2D");
	I::Init(mCreateProgram, "glCreateProgram");
	I::Init(mCreateShader, "glCreateShader");
	I::Init(mCompressedTexSubImage2D, "glCompressedTexSubImage2D");
	I::Init(mDeleteBuffers, "glDeleteBuffers");
	I::Init(mDeleteFramebuffers, "glDeleteFramebuffers");
	I::Init(mDeleteProgram, "glDeleteProgram");
	I::Init(mDeleteRenderbuffers, "glDeleteRenderbuffers");
	I::Init(mDeleteShader, "glDeleteShader");
	I::Init(mDetachShader, "glDetachShader");
	I::Init(mDisableVertexAttribArray, "glDisableVertexAttribArray");
	I::Init(mDrawArrays, "glDrawArrays");
	I::Init(mDrawElements, "glDrawElements");
	I::Init(mEnableVertexAttribArray, "glEnableVertexAttribArray");
	I::Init(mFramebufferRenderbuffer, "glFramebufferRenderbuffer");
	I::Init(mFramebufferTexture2D, "glFramebufferTexture2D");
	I::Init(mGenBuffers, "glGenBuffers");
	I::Init(mGenerateMipmap, "glGenerateMipmap");
	I::Init(mGenFramebuffers, "glGenFramebuffers");
	I::Init(mGenRenderbuffers, "glGenRenderbuffers");
	I::Init(mGetActiveAttrib, "glGetActiveAttrib");
	I::Init(mGetActiveUniform, "glGetActiveUniform");
	I::Init(mGetAttribLocation, "glGetAttribLocation");
	I::Init(mGetProgramiv, "glGetProgramiv");
	I::Init(mGetShaderInfoLog, "glGetShaderInfoLog");
	I::Init(mGetShaderiv, "glGetShaderiv");
	I::Init(mGetUniformLocation, "glGetUniformLocation");
	I::Init(mLinkProgram, "glLinkProgram");
	I::Init(mRenderbufferStorage, "glRenderbufferStorage");
	I::Init(mShaderSource, "glShaderSource");
	I::Init(mTexSubImage2D, "glTexSubImage2D");
	I::Init(mUniform1fv, "glUniform1fv");
	I::Init(mUniform2fv, "glUniform2fv");
	I::Init(mUniform3fv, "glUniform3fv");
	I::Init(mUniform4fv, "glUniform4fv");
	I::Init(mUniform1iv, "glUniform1iv");
	I::Init(mUniform2iv, "glUniform2iv");
	I::Init(mUniform3iv, "glUniform3iv");
	I::Init(mUniform4iv, "glUniform4iv");
	I::Init(mUniformMatrix3fv, "glUniformMatrix3fv");
	I::Init(mUniformMatrix4fv, "glUniformMatrix4fv");
	I::Init(mUseProgram, "glUseProgram");
	I::Init(mVertexAttribPointer, "glVertexAttribPointer");
	I::Init(mValidateProgram, "glValidateProgram");

}

template<class TInitializer>
void OpenGL10ProcAddresses<TInitializer>::Init()
{
	using I = TInitializer;
	I::Init(mClear, "glClear");
	I::Init(mEnable, "glEnable");
	I::Init(mGetIntegerv, "glGetIntegerv");
	I::Init(mReadPixels, "glReadPixels");
	I::Init(mTexImage2D, "glTexImage2D");
	I::Init(mTexParameteri, "glTexParameteri");
	I::Init(mViewport, "glViewport");
}

#endif // GLFUNCTIONSGLES2PROCADDRESSES_H

