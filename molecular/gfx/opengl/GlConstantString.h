/*	GlConstantString.h
	Copyright 2015 Fabian Herb

	This file is part of Molecular Engine.
*/

#ifndef GLCONSTANTSTRING_H
#define GLCONSTANTSTRING_H

#include "OpenGlPrerequisites.h"

/// Returns string for the given OpenGL constant
/** If the constant is unknown, it returns the enum value as a string that is
	only valid until the next call to GlConstantString. */
const char* GlConstantString(GLenum e);

#endif // GLCONSTANTSTRING_H
