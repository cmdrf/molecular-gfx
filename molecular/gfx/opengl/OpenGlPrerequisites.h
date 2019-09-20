/*	OpenGlPrerequisites.h

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

#ifndef MOLECULAR_OPENGLPREREQUISITES_H
#define MOLECULAR_OPENGLPREREQUISITES_H

#include <molecular/Config.h>

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

#endif // MOLECULAR_OPENGLPREREQUISITES_H
