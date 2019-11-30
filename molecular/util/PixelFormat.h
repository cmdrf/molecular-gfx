/*	PixelFormat.h

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

#ifndef MOLECULAR_PIXELFORMAT_H
#define MOLECULAR_PIXELFORMAT_H

#include <ostream>

namespace molecular
{
namespace util
{

/// Pixel formats for textures etc.
enum PixelFormat
{
	PF_NONE,
	PF_UNSUPPORTED,

	PF_ANY, ///< Choose default format

	PF_ANY_RGB,
	PF_ANY_RGBA,
	PF_R8G8B8, ///< 24 bit RGB
	PF_R8G8B8A8, ///< 32 bit RGBA
	PF_B8G8R8, ///< 24 bit BGR
	PF_B8G8R8A8, ///< 24 bit BGRA
	PF_R4G4B4A4, ///< 16 bit RGBA, 4 bit per component
	PF_R5G5B5A1, ///< 15 bit RGB and 1 bit alpha
	PF_L8, ///< 8 bits luminance
	PF_L32_UNSIGNED, ///< 32 bit unsigned integer luminance
	PF_L8A8, ///< 8 bits luminance, 8 bits alpha
	PF_A8, ///< 8 bits alpha

	PF_R8G8B8_SIGNED, ///< Signed integer
	PF_R8G8B8_SIGNED_NORM, ///< Signed normalized
	PF_R8G8B8A8_UNSIGNED, ///< Unsigned integer

	PF_L_FLOAT32, ///< 32 bit float luminance
	PF_RGB_FLOAT16, ///< 16 bit float RGB
	PF_RGB_FLOAT32, ///< 32 bit float RGB
	PF_RGBA_FLOAT16, ///< 16 bit float RGBA
	PF_RGBA_FLOAT32, ///< 32 bit float RGBA

	PF_DEPTH_16, ///< 16 bit integer depth buffer
	PF_DEPTH_24, ///< 24 bit integer depth buffer
	PF_DEPTH_32, ///< 32 bit integer depth buffer
	PF_DEPTH_FLOAT32, ///< 32 bit float depth buffer

	PF_STENCIL_8, ///< 8 bit stencil buffer

	PF_RGB_DXT1, ///< S3 Texture Compression DXT1, SRGB
	PF_RGBA_DXT1, ///< S3TC DXT1 with alpha, SRGB
	PF_RGBA_DXT3, ///< S3TC DXT3, SRGB
	PF_RGBA_DXT5, ///< S3TC DXT5, SRGB

	PF_RGB_ETC1, ///< Ericsson Texture Compression v1, SRGB
	PF_RGB_ETC2, ///< Ericsson Texture Compression v2, SRGB
	PF_RGBA_ETC2, ///< Ericsson Texture Compression v2 with Alpha, SRGB

	PF_RGB_DXT1_LINEAR, ///< S3 Texture Compression DXT1, linear color space
	PF_RGBA_DXT1_LINEAR, ///< S3 Texture Compression DXT1 with alpha, linear color space
	PF_RGBA_DXT3_LINEAR, ///< S3 Texture Compression DXT3, linear color space
	PF_RGBA_DXT5_LINEAR, ///< S3 Texture Compression DXT5, linear color space

	PF_RGB_ETC1_LINEAR, ///< Ericsson Texture Compression v1, linear color space
	PF_RGB_ETC2_LINEAR, ///< Ericsson Texture Compression v2, linear color space
	PF_RGBA_ETC2_LINEAR ///< Ericsson Texture Compression v2 with Alpha, linear color space
};

/// Functions handling PixelFormat values
namespace Pf
{

/// Determine if the given pixel format is a compressed format
bool IsCompressed(PixelFormat format);

}

}
}

std::ostream& operator<<(std::ostream& o, molecular::util::PixelFormat format);

#endif // MOLECULAR_PIXELFORMAT_H
