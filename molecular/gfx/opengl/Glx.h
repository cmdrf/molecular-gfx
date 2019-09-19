/*	Glx.h
	Copyright 2015 Fabian Herb

	This file is part of Molecular Engine.
*/

#ifndef GFX_GLX_H
#define GFX_GLX_H

/// Wrapper around GLX functions
/** Because #including glx.h #includes X11 headers, and those #define all sorts of shit. */
namespace Glx
{
	typedef void (*ProcAddress)(void);
	ProcAddress GetProcAddress(const char* procname);
}

#endif
