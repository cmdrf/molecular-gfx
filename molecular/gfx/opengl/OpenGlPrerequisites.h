/*	OpenGlPrerequisites.h
	Copyright 2014-2017 Fabian Herb

	This file is part of Molecular Engine.
*/

#ifndef OPENGLPREREQUISITES_H
#define OPENGLPREREQUISITES_H

#include <molecular/gfx/Config.h>

#if OPENGL_FOUND
	#if __APPLE__
		#include <OpenGL/gl3.h>
		#include <OpenGL/gl3ext.h>
	#else
		#if _WIN32
			#include <windows.h>
			#include <wingdi.h>
			#include <GL/gl.h>
		#else
			// Linux
			#define GL_GLEXT_PROTOTYPES
			#include <GL/gl.h>
			#include <GL/glext.h>
		#endif
	#endif
	#define GL_APIENTRY APIENTRY
#else
    #if __APPLE__
        #include <OpenGLES/ES3/gl.h>
        #include <OpenGLES/ES3/glext.h>
    #else
        #include <GLES3/gl3.h>
        #include <GLES3/gl3ext.h>
    #endif
	#define OPENGL_ES3
#endif

#endif // OPENGLPREREQUISITES_H
