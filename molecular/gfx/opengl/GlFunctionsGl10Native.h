/*	GlFunctionsGl10Native.h
	Copyright 2015-2017 Fabian Herb

	This file is part of Molecular Engine.
*/

#ifndef GLFUNCTIONSGL10NATIVE_H
#define GLFUNCTIONSGL10NATIVE_H

#include "OpenGlPrerequisites.h"

/// Functions that are in both OpenGL 1.0 and OpenGL ES 2.0
class GlFunctionsGl10NativeNoGles2
{
public:
	void Clear(GLbitfield mask) { glClear(mask); }
	void ClearColor(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha) { glClearColor(red, green, blue, alpha); }
	void DeleteTextures(GLsizei n, GLuint* textures) {glDeleteTextures(n, textures);}
	void Disable(GLenum cap) {glDisable(cap);}
	void Enable(GLenum cap) { glEnable(cap); }
	void GenTextures(GLsizei n, GLuint* textures) {glGenTextures(n, textures);}
	void GetIntegerv(GLenum pname, GLint* params) { glGetIntegerv(pname, params); }
	const GLubyte* GetString(GLenum name) { return glGetString(name); }
	void ReadPixels(GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLvoid* pixels) { glReadPixels(x, y, width, height, format, type, pixels); }
	void TexImage2D(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid* pixels) { glTexImage2D(target, level, internalformat, width, height, border, format, type, pixels); }
	void TexParameteri(GLenum target, GLenum pname, GLint param) { glTexParameteri(target, pname, param); }
	void Viewport(GLint x, GLint y, GLsizei width, GLsizei height) { glViewport(x, y, width, height); }
};

#if OPENGL_FOUND
/// Functions that are in OpenGL 1.0
/** Extends GlFunctionsGl10NativeNoGles2 by functions that are in OpenGL 1.0,
	but not in OpenGL ES 2.0 */
class GlFunctionsGl10Native : public GlFunctionsGl10NativeNoGles2
{
public:
	void ReadBuffer(GLenum mode) { glReadBuffer(mode); }

	void TexImage1D(GLenum target, GLint level, GLint internalformat, GLsizei width, GLint border, GLenum format, GLenum type, const GLvoid* pixels) { glTexImage1D(target, level, internalformat, width, border, format, type, pixels); }
};
#endif

#endif
