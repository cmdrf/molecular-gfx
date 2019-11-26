/*	PixelFormatConversion.h

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
SOFTWARE.*/

#ifndef MOLECULAR_PIXELFORMATCONVERSION_H
#define MOLECULAR_PIXELFORMATCONVERSION_H

#include <molecular/util/PixelFormat.h>

namespace molecular
{
namespace gfx
{

/// Conversion between PixelFormat and OpenGL type, format and internalFormat
/** Also used outside of the OpenGL module, so this must not depend on OpenGL
    headers. Hence the use of uint32_t instead of glEnum. */
namespace PixelFormatConversion
{
uint32_t ToGlType(util::PixelFormat format);
uint32_t ToGlFormat(util::PixelFormat format);
uint32_t ToGlInternalFormat(util::PixelFormat format);

/** Only implemented for a few compressed types. */
util::PixelFormat ToPixelFormat(uint32_t glType, uint32_t glFormat, uint32_t glInternalFormat);
}

}
}

#endif // MOLECULAR_PIXELFORMATCONVERSION_H
