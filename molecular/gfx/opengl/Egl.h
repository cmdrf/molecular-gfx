/*	Egl.h
	Copyright 2015 Fabian Herb

	This file is part of Molecular Engine.
*/

#ifndef GFX_EGL_H
#define GFX_EGL_H

/// Wrapper around EGL functions
/** Because #including egl.h #includes X11 headers, and those #define all sorts of shit. */
namespace Egl
{
	typedef void (*ProcAddress)(void);
	ProcAddress GetProcAddress(const char* procname);
}

#endif
