/*	PixelFormatConversion.h
        Copyright 2015-2017 Fabian Herb

	This file is part of Molecular Engine.
*/

#ifndef PIXELFORMATCONVERSION_H
#define PIXELFORMATCONVERSION_H

#include <molecular/util/PixelFormat.h>

/// Conversion between PixelFormat and OpenGL type, format and internalFormat
/** Also used outside of the OpenGL module, so this must not depend on OpenGL
    headers. Hence the use of uint32_t instead of glEnum. */
namespace PixelFormatConversion
{
uint32_t ToGlType(PixelFormat format);
uint32_t ToGlFormat(PixelFormat format);
uint32_t ToGlInternalFormat(PixelFormat format);

/** Only implemented for a few compressed types. */
PixelFormat ToPixelFormat(uint32_t glType, uint32_t glFormat, uint32_t glInternalFormat);
}

#endif // PIXELFORMATCONVERSION_H
