#ifndef GLFUNCTIONSNATIVE_H
#define GLFUNCTIONSNATIVE_H

#include "OpenGlPrerequisites.h"

/// Native OpenGL calls above ES 2.0
/** Used on Apple. Also works in conjunction with GLEW or GL3W. */
class GlFunctionsNative
{
public:
	void Init() {}

	inline void BeginQuery(GLenum target, GLuint id) { glBeginQuery(target, id); }
	inline void BeginTransformFeedback(GLenum primitiveMode) { glBeginTransformFeedback(primitiveMode); }
	inline void BindBufferBase(GLenum target, GLuint index, GLuint buffer) { glBindBufferBase(target, index, buffer); }
	inline void BindFragDataLocation(GLuint program, GLuint color, const GLchar * name) { glBindFragDataLocation(program, color, name); }
	inline void BindTransformFeedback(GLenum target, GLuint id) { glBindTransformFeedback(target, id); }
	inline void BindVertexArray(GLuint array) { glBindVertexArray(array); }
	inline void BlitFramebuffer(GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield mask, GLenum filter) {glBlitFramebuffer(srcX0, srcY0, srcX1, srcY1, dstX0, dstY0, dstX1, dstY1, mask, filter);}
	inline void DeleteQueries(GLsizei n, const GLuint* ids) { glDeleteQueries(n, ids); }
	inline void DeleteTransformFeedbacks(GLsizei n, GLuint *arrays) { glDeleteTransformFeedbacks(n, arrays); }
	inline void DeleteVertexArrays(GLsizei n, GLuint* arrays) { glGenVertexArrays(n, arrays); }
	inline void DrawBuffers(GLsizei n, const GLenum* bufs) { glDrawBuffers(n, bufs); }
	inline void DrawRangeElements(GLenum mode, GLuint start, GLuint end, GLsizei count, GLenum type, const GLvoid* indices) { glDrawRangeElements(mode, start, end, count, type, indices); }
	inline void EndQuery(GLenum target) { glEndQuery(target); }
	inline void EndTransformFeedback() { glEndTransformFeedback(); }
	inline void FramebufferTexture3D(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level, GLint layer) { glFramebufferTexture3D(target, attachment, textarget, texture, level, layer); }
	inline void GenQueries(GLsizei n, GLuint* ids) { glGenQueries(n, ids); }
	inline void GenTransformFeedbacks(GLsizei n, GLuint* arrays) { glGenTransformFeedbacks(n, arrays); }
	inline void GenVertexArrays(GLsizei n, GLuint* arrays) { glGenVertexArrays(n, arrays); }
	inline void GetActiveUniformsiv(GLuint program, GLsizei uniformCount, const GLuint *uniformIndices, GLenum pname, GLint *params) {glGetActiveUniformsiv(program, uniformCount, uniformIndices, pname, params);}
	inline void GetBufferParameteri64v(GLenum target, GLenum value, GLint64* data) { glGetBufferParameteri64v(target, value, data); }
	inline void GetFramebufferAttachmentParameteriv(GLenum target, GLenum attachment, GLenum pname, GLint *params) {glGetFramebufferAttachmentParameteriv(target, attachment, pname, params);}
	inline void GetQueryiv(GLenum target, GLenum pname, GLint* params) { glGetQueryiv(target, pname, params); }
	inline void GetQueryObjectiv(GLuint id, GLenum pname, GLint* params) { glGetQueryObjectiv(id, pname, params); }
	inline void GetQueryObjectuiv(GLuint id, GLenum pname, GLuint* params) { glGetQueryObjectuiv(id, pname, params); }
	inline const GLubyte* GetStringi(GLenum name, GLuint index) { return glGetStringi(name, index); }
	inline void GetTransformFeedbackVarying(GLuint program, GLuint index, GLsizei bufSize, GLsizei* length, GLsizei* size, GLenum* type, char* name) { glGetTransformFeedbackVarying(program, index, bufSize, length, size, type, name); }
	inline void PauseTransformFeedback() { glPauseTransformFeedback(); }
	inline void ReadBuffer(GLenum mode) { glReadBuffer(mode); }
	inline void RenderbufferStorageMultisample(GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height) {glRenderbufferStorageMultisample(target, samples, internalformat, width, height);}
	inline void ResumeTransformFeedback() { glResumeTransformFeedback(); }
	inline void TexImage2DMultisample(GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLboolean fixedsamplelocations) {glTexImage2DMultisample(target, samples, internalformat, width, height, fixedsamplelocations);}
	inline void TexImage3D(GLenum target, GLint level, GLint internalFormat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const GLvoid * data) { glTexImage3D(target, level, internalFormat, width, height, depth, border, format, type, data); }
	inline void TransformFeedbackVaryings(GLuint program, GLsizei count, const char** varyings, GLenum bufferMode) { glTransformFeedbackVaryings(program, count, varyings, bufferMode); }
	inline void Uniform1ui(GLint location, GLuint v0) { glUniform1ui(location, v0); }
	inline void Uniform2ui(GLint location, GLuint v0, GLuint v1) { glUniform2ui(location, v0, v1); }
	inline void Uniform3ui(GLint location, GLuint v0, GLuint v1, GLuint v2) { glUniform3ui(location, v0, v1, v2); }
	inline void Uniform4ui(GLint location, GLuint v0, GLuint v1, GLuint v2, GLuint v3) { glUniform4ui(location, v0, v1, v2, v3); }
	inline void Uniform1uiv(GLint location, GLsizei count, const GLuint* value) { glUniform1uiv(location, count, value); }
	inline void Uniform2uiv(GLint location, GLsizei count, const GLuint* value) { glUniform2uiv(location, count, value); }
	inline void Uniform3uiv(GLint location, GLsizei count, const GLuint* value) { glUniform3uiv(location, count, value); }
	inline void Uniform4uiv(GLint location, GLsizei count, const GLuint* value) { glUniform4uiv(location, count, value); }
	inline void UniformBlockBinding(GLuint program, GLuint uniformBlockIndex, GLuint uniformBlockBinding) { glUniformBlockBinding(program, uniformBlockIndex, uniformBlockBinding); }
	inline void VertexAttribIPointer(GLuint index, GLint size, GLenum type, GLsizei stride, const void *pointer) { glVertexAttribIPointer(index, size, type, stride, pointer); }

	inline void DrawTransformFeedback(GLenum mode, GLuint tf) { glDrawTransformFeedback(mode, tf); }
	inline void PrimitiveRestartIndex(GLuint index) { glPrimitiveRestartIndex(index); }

	/** @todo Check on Apple GLES3 */
	bool HasBindFragDataLocation() const { return true; }

	/** @todo Check on Apple GLES3 */
	bool HasDrawTransformFeedback() const { return true; }

	/** @todo Check on Apple GLES3 */
	bool HasPrimitiveRestartIndex() const { return true; }
};


#endif
