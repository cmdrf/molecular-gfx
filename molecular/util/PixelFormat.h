/*	PixelFormat.h
	Copyright 2012-2015 Fabian Herb

	This file is part of Molecular Engine.
*/

#ifndef PIXELFORMAT_H
#define PIXELFORMAT_H

#include <ostream>

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
	PF_R4G4B4A4,
	PF_R5G5B5A1,
	PF_L8, ///< 8 bits luminance
	PF_L32_UNSIGNED, ///< 32 bit unsigned integer luminance
	PF_L8A8, ///< 8 bits luminance, 8 bits alpha
	PF_A8, ///< 8 bits alpha

	PF_R8G8B8_SIGNED, ///< Signed integer
	PF_R8G8B8_SIGNED_NORM, ///< Signed normalized
	PF_R8G8B8A8_UNSIGNED, ///< Unsigned integer

	PF_L_FLOAT32, ///< 32 bit float luminance
	PF_RGB_FLOAT16, ///< 16 bit float RGB
	PF_RGB_FLOAT32,
	PF_RGBA_FLOAT16,
	PF_RGBA_FLOAT32,

	PF_DEPTH_16,
	PF_DEPTH_24,
	PF_DEPTH_32,
	PF_DEPTH_FLOAT32,

	PF_STENCIL_8,

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

namespace Pf
{

	bool IsCompressed(PixelFormat format);

}

std::ostream& operator<<(std::ostream& o, PixelFormat format);

#endif // PIXELFORMAT_H
