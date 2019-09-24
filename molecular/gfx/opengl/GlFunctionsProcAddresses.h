/*	GlFunctionsProcAddresses.h

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

#ifndef MOLECULAR_GLFUNCTIONSPROCADDRESSES_H
#define MOLECULAR_GLFUNCTIONSPROCADDRESSES_H

#include "OpenGlPrerequisites.h"
#include <cassert>
#include <stdint.h>

namespace molecular
{

/// Procedure addresses for functions above GLES 2.0
template<class TInitializer>
class GlFunctionsProcAddresses
{
public:
	/** Required on Windows. */
	typedef int64_t GLint64;

	/** Required on Windows. */
	typedef char GLchar;

	void Init();

	inline void BeginQuery(GLenum target, GLuint id) {mBeginQuery(target, id);}
	inline void BeginTransformFeedback(GLenum primitiveMode) {mBeginTransformFeedback(primitiveMode);}
	inline void BindBufferBase(GLenum target, GLuint index, GLuint buffer) {mBindBufferBase(target, index, buffer);}
	inline void BindFragDataLocation(GLuint program, GLuint color, const GLchar * name) { assert(mBindFragDataLocation); mBindFragDataLocation(program, color, name); }
	inline void BindTransformFeedback(GLenum target, GLuint id) {mBindTransformFeedback(target, id);}
	inline void BindVertexArray (GLuint array) {mBindVertexArray(array);}
	inline void BlitFramebuffer(GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield mask, GLenum filter) {assert(mBlitFramebuffer); mBlitFramebuffer(srcX0, srcY0, srcX1, srcY1, dstX0, dstY0, dstX1, dstY1, mask, filter);}
	inline void DeleteQueries(GLsizei n, const GLuint * ids) {mDeleteQueries(n, ids);}
	inline void DeleteTransformFeedbacks(GLsizei n, GLuint *arrays) {mDeleteTransformFeedbacks(n, arrays);}
	inline void DeleteVertexArrays(GLsizei n, GLuint *arrays) {mDeleteVertexArrays(n, arrays);}
	inline void DrawBuffers (GLsizei n, const GLenum *bufs) {mDrawBuffers(n, bufs);}
	inline void DrawRangeElements(GLenum mode, GLuint start, GLuint end, GLsizei count, GLenum type, const GLvoid* indices) {mDrawRangeElements(mode, start, end, count, type, indices);}
	inline void EndQuery(GLenum target) {mEndQuery(target);}
	inline void EndTransformFeedback() {mEndTransformFeedback();}
	inline void FramebufferTexture3D(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level, GLint layer) {mFramebufferTexture3D(target, attachment, textarget, texture, level, layer);}
	inline void GenQueries(GLsizei n, GLuint* ids) {mGenQueries(n, ids);}
	inline void GenTransformFeedbacks(GLsizei n, GLuint *bufs) {mGenTransformFeedbacks(n, bufs);}
	inline void GenVertexArrays(GLsizei n, GLuint *arrays) {assert(mGenVertexArrays); mGenVertexArrays(n, arrays);}
	inline void GetActiveUniformsiv(GLuint program, GLsizei uniformCount, const GLuint *uniformIndices, GLenum pname, GLint *params) {assert(mGetActiveUniformsiv); mGetActiveUniformsiv(program, uniformCount, uniformIndices, pname, params);}
	inline void GetBufferParameteri64v(GLenum target, GLenum value, GLint64* data) {assert(mGetBufferParameteri64v); mGetBufferParameteri64v(target, value, data);}
	inline void GetFramebufferAttachmentParameteriv(GLenum target, GLenum attachment, GLenum pname, GLint *params) {assert(mGetFramebufferAttachmentParameteriv); mGetFramebufferAttachmentParameteriv(target, attachment, pname, params);}
	inline void GetQueryiv(GLenum target, GLenum pname, GLint* params) {mGetQueryiv(target, pname, params);}
	inline void GetQueryObjectiv(GLuint id, GLenum pname, GLint* params) {assert(mGetQueryObjectiv); mGetQueryObjectiv(id, pname, params);}
	inline void GetQueryObjectuiv(GLuint id, GLenum pname, GLuint* params) {mGetQueryObjectuiv(id, pname, params);}
	inline const GLubyte* GetStringi(GLenum name, GLuint index) {return mGetStringi(name, index);}
	inline void GetTransformFeedbackVarying(GLuint program, GLuint index, GLsizei bufSize, GLsizei* length, GLsizei* size, GLenum* type, char* name) {mGetTransformFeedbackVarying(program, index, bufSize, length, size, type, name);}
	inline void PauseTransformFeedback() {mPauseTransformFeedback();}
	inline void ReadBuffer(GLenum mode) { assert(mReadBuffer); mReadBuffer(mode); }
	inline void RenderbufferStorageMultisample(GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height) {mRenderbufferStorageMultisample(target, samples, internalformat, width, height);}
	inline void ResumeTransformFeedback() {mResumeTransformFeedback();}
	inline void TexImage2DMultisample(GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLboolean fixedsamplelocations) {assert(mTexImage2DMultisample); mTexImage2DMultisample(target, samples, internalformat, width, height, fixedsamplelocations);}
	inline void TexImage3D(GLenum target, GLint level, GLint internalFormat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const GLvoid * data) {mTexImage3D(target, level, internalFormat, width, height, depth, border, format, type, data);}
	inline void TransformFeedbackVaryings(GLuint program, GLsizei count, const char ** varyings, GLenum bufferMode) {mTransformFeedbackVaryings(program, count, varyings, bufferMode);}
	inline void Uniform1ui (GLint location, GLuint v0) {mUniform1ui(location, v0);}
	inline void Uniform2ui (GLint location, GLuint v0, GLuint v1) {mUniform2ui(location, v0, v1);}
	inline void Uniform3ui (GLint location, GLuint v0, GLuint v1, GLuint v2)  {mUniform3ui(location, v0, v1, v2);}
	inline void Uniform4ui (GLint location, GLuint v0, GLuint v1, GLuint v2, GLuint v3)  {mUniform4ui(location, v0, v1, v2, v3);}
	inline void Uniform1uiv (GLint location, GLsizei count, const GLuint *value) {mUniform1uiv(location, count, value);}
	inline void Uniform2uiv (GLint location, GLsizei count, const GLuint *value) {mUniform2uiv(location, count, value);}
	inline void Uniform3uiv (GLint location, GLsizei count, const GLuint *value) {mUniform3uiv(location, count, value);}
	inline void Uniform4uiv (GLint location, GLsizei count, const GLuint *value) {mUniform4uiv(location, count, value);}
	inline void UniformBlockBinding(GLuint program, GLuint uniformBlockIndex, GLuint uniformBlockBinding) {mUniformBlockBinding(program, uniformBlockIndex, uniformBlockBinding);}
	inline void VertexAttribIPointer (GLuint index, GLint size, GLenum type, GLsizei stride, const void *pointer) {mVertexAttribIPointer(index, size, type, stride, pointer);}

	// Extensions:
	inline void DrawTransformFeedback(GLenum mode, GLuint tf) {assert(mDrawTransformFeedback); mDrawTransformFeedback(mode, tf);}
	inline void PrimitiveRestartIndex(GLuint index) {assert(mPrimitiveRestartIndex); mPrimitiveRestartIndex(index);}

	bool HasBindFragDataLocation() { return mBindFragDataLocation != nullptr; }
	bool HasDrawTransformFeedback() {return mDrawTransformFeedback != nullptr;}
	bool HasPrimitiveRestartIndex() { return mPrimitiveRestartIndex != nullptr; }

protected:
	using BeginQueryType = void (GL_APIENTRY*) (GLenum target, GLuint id);
	using BeginTransformFeedbackType = void (GL_APIENTRY*)(GLenum primitiveMode);
	using BindBufferBaseType = void (GL_APIENTRY*) (GLenum, GLuint, GLuint);
	using BindFragDataLocationType = void (GL_APIENTRY *)(GLuint, GLuint, const GLchar*);
	using BindTransformFeedbackType = void (GL_APIENTRY*) (GLenum, GLuint);
	using BindVertexArrayType = void (GL_APIENTRY*) (GLuint array);
	using BlitFramebufferType = void (GL_APIENTRY*) (GLint, GLint, GLint, GLint, GLint, GLint, GLint, GLint, GLbitfield, GLenum);
	using DeleteQueriesType = void (GL_APIENTRY*) (GLsizei, const GLuint*);
	using DeleteTransformFeedbacksType = void (GL_APIENTRY*) (GLsizei n, GLuint *arrays);
	using DeleteVertexArraysType = void (GL_APIENTRY*) (GLsizei n, GLuint *arrays);
	using DrawBuffersType = void (GL_APIENTRY*) (GLsizei n, const GLenum *bufs);
	using DrawRangeElementsType = void (GL_APIENTRY*) (GLenum, GLuint start, GLuint, GLsizei, GLenum, const GLvoid*);
	using EndQueryType = void (GL_APIENTRY*)(GLenum target);
	using EndTransformFeedbackType = void (GL_APIENTRY*) (void);
	using FramebufferTexture3DType = void (GL_APIENTRY*) (GLenum, GLenum, GLenum, GLuint, GLint, GLint);
	using GenQueriesType = void (GL_APIENTRY*) (GLsizei, GLuint*);
	using GenTransformFeedbacksType = void (GL_APIENTRY*) (GLsizei n, GLuint *arrays);
	using GenVertexArraysType= void (GL_APIENTRY*) (GLsizei n, GLuint *arrays);
	using GetActiveUniformsivType = void (GL_APIENTRY*) (GLuint program, GLsizei uniformCount, const GLuint *uniformIndices, GLenum pname, GLint *params);
	using GetBufferParameteri64vType = void (GL_APIENTRY*) (GLenum, GLenum, GLint64*);
	using GetFramebufferAttachmentParameterivType = void (GL_APIENTRY*)(GLenum, GLenum, GLenum, GLint*);
	using GetQueryivType = void (GL_APIENTRY*) (GLenum, GLenum, GLint*);
	typedef void (GL_APIENTRY *GetQueryObjectivType) (GLuint id, GLenum pname, GLint* params);
	typedef void (GL_APIENTRY *GetQueryObjectuivType) (GLuint id, GLenum pname, GLuint* params);
	typedef const GLubyte* (GL_APIENTRY *GetStringiType)(GLenum name, GLuint index);
	typedef void (GL_APIENTRY *GetTransformFeedbackVaryingType)(GLuint program, GLuint index, GLsizei bufSize, GLsizei* length, GLsizei* size, GLenum* type, char* name);
	typedef void (GL_APIENTRY *PauseTransformFeedbackType) (void);
	typedef void (GL_APIENTRY *ReadBufferType) (GLenum mode);
	using RenderbufferStorageMultisampleType = void (GL_APIENTRY*)(GLenum, GLsizei, GLenum, GLsizei, GLsizei);
	typedef void (GL_APIENTRY *ResumeTransformFeedbackType) (void);
	using TexImage2DMultisampleType = void (GL_APIENTRY*) (GLenum, GLsizei, GLenum, GLsizei, GLsizei, GLboolean);
	typedef void (GL_APIENTRY *TexImage3DType) (GLenum, GLint, GLint, GLsizei, GLsizei, GLsizei, GLint, GLenum, GLenum, const GLvoid*);
	typedef void (GL_APIENTRY *TexStorage2DType) (GLenum, GLsizei, GLenum, GLsizei, GLsizei);
	typedef void (GL_APIENTRY *TexStorage3DType) (GLenum, GLsizei, GLenum, GLsizei, GLsizei, GLsizei);
	typedef void (GL_APIENTRY *TransformFeedbackVaryingsType) (GLuint program, GLsizei count, const char** varyings, GLenum bufferMode);
	typedef void (GL_APIENTRY *Unform1uiType) (GLint, GLuint);
	typedef void (GL_APIENTRY *Unform2uiType) (GLint, GLuint, GLuint);
	typedef void (GL_APIENTRY *Unform3uiType) (GLint, GLuint, GLuint, GLuint);
	typedef void (GL_APIENTRY *Unform4uiType) (GLint, GLuint, GLuint, GLuint, GLuint);
	typedef void (GL_APIENTRY *UniformBlockBindingType) (GLuint, GLuint, GLuint);
	typedef void (GL_APIENTRY *VertexAttribIPointerType) (GLuint index, GLint size, GLenum type, GLsizei stride, const void *pointer);

	/// Function pointer type to glUniform1uiv through glUniform4uiv
	typedef void (GL_APIENTRY *UniformXuivType) (GLint location, GLsizei count, const GLuint *value);

	// Extensions:
	typedef void (GL_APIENTRY *DrawTransformFeedbackType) (GLenum, GLuint);
	using PrimitiveRestartIndexType = void (GL_APIENTRY*)(GLuint);

	BeginQueryType mBeginQuery = nullptr;
	BeginTransformFeedbackType mBeginTransformFeedback = nullptr;
	BindBufferBaseType mBindBufferBase = nullptr;
	BindFragDataLocationType mBindFragDataLocation = nullptr;
	BindTransformFeedbackType mBindTransformFeedback = nullptr;
	BindVertexArrayType mBindVertexArray = nullptr;
	BlitFramebufferType mBlitFramebuffer = nullptr;
	DeleteQueriesType mDeleteQueries = nullptr;
	DeleteTransformFeedbacksType mDeleteTransformFeedbacks = nullptr;
	DeleteVertexArraysType mDeleteVertexArrays = nullptr;
	DrawBuffersType mDrawBuffers = nullptr;
	DrawRangeElementsType mDrawRangeElements = nullptr;
	EndQueryType mEndQuery = nullptr;
	EndTransformFeedbackType mEndTransformFeedback = nullptr;
	FramebufferTexture3DType mFramebufferTexture3D = nullptr;
	GenQueriesType mGenQueries = nullptr;
	GenTransformFeedbacksType mGenTransformFeedbacks = nullptr;
	GenVertexArraysType mGenVertexArrays = nullptr;
	GetActiveUniformsivType mGetActiveUniformsiv = nullptr;
	GetBufferParameteri64vType mGetBufferParameteri64v = nullptr;
	GetFramebufferAttachmentParameterivType mGetFramebufferAttachmentParameteriv = nullptr;
	GetQueryivType mGetQueryiv = nullptr;
	GetQueryObjectivType mGetQueryObjectiv = nullptr;
	GetQueryObjectuivType mGetQueryObjectuiv = nullptr;
	GetStringiType mGetStringi = nullptr;
	GetTransformFeedbackVaryingType mGetTransformFeedbackVarying = nullptr;
	PauseTransformFeedbackType mPauseTransformFeedback = nullptr;
	ReadBufferType mReadBuffer = nullptr;
	RenderbufferStorageMultisampleType mRenderbufferStorageMultisample = nullptr;
	ResumeTransformFeedbackType mResumeTransformFeedback = nullptr;
	TexImage2DMultisampleType mTexImage2DMultisample = nullptr;
	TexImage3DType mTexImage3D = nullptr;
	TexStorage2DType mTexStorage2D = nullptr;
	TexStorage3DType mTexStorage3D = nullptr;
	TransformFeedbackVaryingsType mTransformFeedbackVaryings = nullptr;
	Unform1uiType mUniform1ui = nullptr;
	Unform2uiType mUniform2ui = nullptr;
	Unform3uiType mUniform3ui = nullptr;
	Unform4uiType mUniform4ui = nullptr;
	UniformXuivType mUniform1uiv = nullptr;
	UniformXuivType mUniform2uiv = nullptr;
	UniformXuivType mUniform3uiv = nullptr;
	UniformXuivType mUniform4uiv = nullptr;
	UniformBlockBindingType mUniformBlockBinding = nullptr;
	VertexAttribIPointerType mVertexAttribIPointer = nullptr;

	// Extensions:
	DrawTransformFeedbackType mDrawTransformFeedback = nullptr;
	PrimitiveRestartIndexType mPrimitiveRestartIndex = nullptr;
};

/*****************************************************************************/

template<class TInitializer>
void GlFunctionsProcAddresses<TInitializer>::Init()
{
	using I = TInitializer;
	I::Init(mBeginQuery, "glBeginQuery");
	I::Init(mBeginTransformFeedback, "glBeginTransformFeedback");
	I::Init(mBindBufferBase, "glBindBufferBase");
	I::Init(mBindFragDataLocation, "glBindFragDataLocation", true);
	I::Init(mBindTransformFeedback, "glBindTransformFeedback");
	I::Init(mBindVertexArray, "glBindVertexArray");
	I::Init(mBlitFramebuffer, "glBlitFramebuffer");
	I::Init(mDeleteQueries, "glDeleteQueries");
	I::Init(mDeleteTransformFeedbacks, "glDeleteTransformFeedbacks");
	I::Init(mDeleteVertexArrays, "glDeleteVertexArrays");
	I::Init(mDrawBuffers, "glDrawBuffers");
	I::Init(mDrawRangeElements, "glDrawRangeElements");
	I::Init(mEndQuery, "glEndQuery");
	I::Init(mEndTransformFeedback, "glEndTransformFeedback");
	I::Init(mFramebufferTexture3D, "glFramebufferTexture3D", false, "glFramebufferTexture3DOES");
	I::Init(mGenQueries, "glGenQueries");
	I::Init(mGenTransformFeedbacks, "glGenTransformFeedbacks");
	I::Init(mGenVertexArrays, "glGenVertexArrays");
	I::Init(mGetActiveUniformsiv, "glGetActiveUniformsiv");
	I::Init(mGetBufferParameteri64v, "glGetBufferParameteri64v");
	I::Init(mGetFramebufferAttachmentParameteriv, "glGetFramebufferAttachmentParameteriv");
	I::Init(mGetQueryiv, "glGetQueryiv");
	I::Init(mGetQueryObjectiv, "glGetQueryObjectiv", true);
	I::Init(mGetQueryObjectuiv, "glGetQueryObjectuiv");
	I::Init(mGetStringi, "glGetStringi");
	I::Init(mGetTransformFeedbackVarying, "glGetTransformFeedbackVarying");
	I::Init(mPauseTransformFeedback, "glPauseTransformFeedback");
	I::Init(mReadBuffer, "glReadBuffer", true); // TODO: Native on windows
	I::Init(mRenderbufferStorageMultisample, "glRenderbufferStorageMultisample");
	I::Init(mResumeTransformFeedback, "glResumeTransformFeedback");
	I::Init(mTexImage2DMultisample, "glTexImage2DMultisample");
	I::Init(mTexImage3D, "glTexImage3D");
	I::Init(mTransformFeedbackVaryings, "glTransformFeedbackVaryings");
	I::Init(mUniform1ui, "glUniform1ui");
	I::Init(mUniform2ui, "glUniform2ui");
	I::Init(mUniform3ui, "glUniform3ui");
	I::Init(mUniform4ui, "glUniform4ui");
	I::Init(mUniform1uiv, "glUniform1uiv");
	I::Init(mUniform2uiv, "glUniform2uiv");
	I::Init(mUniform3uiv, "glUniform3uiv");
	I::Init(mUniform4uiv, "glUniform4uiv");
	I::Init(mUniformBlockBinding, "glUniformBlockBinding");
	I::Init(mVertexAttribIPointer, "glVertexAttribIPointer");

	// Extensions:
	I::Init(mDrawTransformFeedback, "glDrawTransformFeedback", true);
	I::Init(mPrimitiveRestartIndex, "glPrimitiveRestartIndex", true);
}

}

#endif // GLFUNCTIONSGLES3_H
