/*	PixelFormatConversion.cpp

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

#include "PixelFormatConversion.h"
#include "GlConstants.h"

namespace molecular
{
namespace PixelFormatConversion
{

using Gl = GlConstants;

uint32_t ToGlType(PixelFormat format)
{
	switch(format)
	{
	case PF_R8G8B8:
	case PF_R8G8B8A8:
	case PF_B8G8R8:
	case PF_B8G8R8A8:
	case PF_L8:
	case PF_L8A8:
	case PF_A8:
	case PF_STENCIL_8:
	case PF_R8G8B8_SIGNED_NORM:
                return Gl::UNSIGNED_BYTE;

        case PF_L32_UNSIGNED: return Gl::UNSIGNED_INT;

        case PF_R8G8B8_SIGNED: return Gl::BYTE;

	case PF_R4G4B4A4: return Gl::UNSIGNED_SHORT_4_4_4_4;
	case PF_R5G5B5A1: return Gl::UNSIGNED_SHORT_5_5_5_1;

	case PF_L_FLOAT32:
	case PF_RGB_FLOAT32:
	case PF_RGBA_FLOAT32:
				return Gl::FLOAT;

	case PF_RGB_FLOAT16:
	case PF_RGBA_FLOAT16:
		return Gl::HALF_FLOAT;

        case PF_DEPTH_16: return Gl::UNSIGNED_SHORT;
//	case PF_DEPTH_24: return Gl::UNSIGNED_INT_24_8;
        case PF_DEPTH_24: return Gl::UNSIGNED_INT;
        case PF_DEPTH_32: return Gl::UNSIGNED_INT;

	case PF_R8G8B8A8_UNSIGNED:
		return Gl::UNSIGNED_INT_8_8_8_8;

	default:
		return 0;
	}
}

uint32_t ToGlFormat(PixelFormat format)
{
	switch(format)
	{
	case PF_ANY_RGB:
	case PF_R8G8B8:
	case PF_RGB_FLOAT16:
	case PF_RGB_FLOAT32:
                return Gl::RGB;

	case PF_R8G8B8_SIGNED_NORM:
	case PF_R8G8B8_SIGNED:
		return Gl::RGB_INTEGER;

	case PF_ANY_RGBA:
	case PF_R4G4B4A4:
	case PF_R8G8B8A8:
	case PF_R5G5B5A1:
	case PF_RGBA_FLOAT16:
	case PF_RGBA_FLOAT32:
                return Gl::RGBA;

	case PF_R8G8B8A8_UNSIGNED:
		return Gl::RGBA_INTEGER;

	case PF_L32_UNSIGNED:
		return Gl::RED_INTEGER;

	case PF_L_FLOAT32:
	case PF_L8:
                return Gl::RED;

	case PF_L8A8: return Gl::RG;
        case PF_A8: return Gl::RED;

	case PF_DEPTH_16:
	case PF_DEPTH_24:
	case PF_DEPTH_32:
                return Gl::DEPTH_COMPONENT;

	case PF_B8G8R8: return Gl::BGR;
	case PF_B8G8R8A8: return Gl::BGRA;

	default:
		return 0;
	}
}

uint32_t ToGlInternalFormat(PixelFormat format)
{
	switch(format)
	{
        case PF_ANY_RGB: return Gl::RGB;
        case PF_ANY_RGBA: return Gl::RGBA;

	case PF_R8G8B8:
	case PF_B8G8R8:
                return Gl::RGB8;

	case PF_R8G8B8A8:
	case PF_B8G8R8A8:
                return Gl::RGBA8;

	case PF_R8G8B8_SIGNED_NORM: return Gl::RGB8_SNORM;
	case PF_R8G8B8A8_UNSIGNED: return Gl::RGBA8UI;
	case PF_R8G8B8_SIGNED: return Gl::RGB8I;

	case PF_L8: return Gl::R8;
	case PF_L32_UNSIGNED: return Gl::R32UI;

	case PF_RGB_FLOAT16: return Gl::RGB16F;
	case PF_RGB_FLOAT32: return Gl::RGB32F;
	case PF_RGBA_FLOAT16: return Gl::RGBA16F;
	case PF_RGBA_FLOAT32: return Gl::RGBA32F;
	case PF_L_FLOAT32: return Gl::R32F;
	case PF_DEPTH_16: return Gl::DEPTH_COMPONENT16;
	case PF_DEPTH_24: return Gl::DEPTH_COMPONENT24;
	case PF_DEPTH_FLOAT32: return Gl::DEPTH_COMPONENT32F;
	case PF_DEPTH_32: return Gl::DEPTH_COMPONENT32;

	case PF_RGB_DXT1_LINEAR: return Gl::COMPRESSED_RGB_S3TC_DXT1_EXT;
	case PF_RGBA_DXT1_LINEAR: return Gl::COMPRESSED_RGBA_S3TC_DXT1_EXT;
	case PF_RGBA_DXT3_LINEAR: return Gl::COMPRESSED_RGBA_S3TC_DXT3_EXT;
	case PF_RGBA_DXT5_LINEAR: return Gl::COMPRESSED_RGBA_S3TC_DXT5_EXT;

	case PF_RGB_DXT1: return Gl::COMPRESSED_SRGB_S3TC_DXT1_EXT;
	case PF_RGBA_DXT1: return Gl::COMPRESSED_SRGB_ALPHA_S3TC_DXT1_EXT;
	case PF_RGBA_DXT3: return Gl::COMPRESSED_SRGB_ALPHA_S3TC_DXT3_EXT;
	case PF_RGBA_DXT5: return Gl::COMPRESSED_SRGB_ALPHA_S3TC_DXT5_EXT;

	case PF_RGB_ETC1: // Any ETC1 image is also valid ETC2
	case PF_RGB_ETC2:
		return Gl::COMPRESSED_SRGB8_ETC2;
	case PF_RGBA_ETC2: return Gl::COMPRESSED_SRGB8_ALPHA8_ETC2_EAC;

	case PF_RGB_ETC1_LINEAR: // Any ETC1 image is also valid ETC2
	case PF_RGB_ETC2_LINEAR:
		return Gl::COMPRESSED_RGB8_ETC2;
	case PF_RGBA_ETC2_LINEAR: return Gl::COMPRESSED_RGBA8_ETC2_EAC;

	default:
		return ToGlFormat(format);
	}
}

PixelFormat ToPixelFormat(uint32_t glType, uint32_t glFormat, uint32_t glInternalFormat)
{
	switch(glInternalFormat)
	{
	case Gl::COMPRESSED_RGB_S3TC_DXT1_EXT:
	case Gl::COMPRESSED_SRGB_S3TC_DXT1_EXT:
		return PF_RGB_DXT1;
	case Gl::COMPRESSED_RGBA_S3TC_DXT1_EXT:
	case Gl::COMPRESSED_SRGB_ALPHA_S3TC_DXT1_EXT:
		return PF_RGBA_DXT1;
	case Gl::COMPRESSED_RGBA_S3TC_DXT3_EXT:
	case Gl::COMPRESSED_SRGB_ALPHA_S3TC_DXT3_EXT:
		return PF_RGBA_DXT3;
	case Gl::COMPRESSED_RGBA_S3TC_DXT5_EXT:
	case Gl::COMPRESSED_SRGB_ALPHA_S3TC_DXT5_EXT:
		return PF_RGBA_DXT5;
	case Gl::COMPRESSED_RGB8_ETC2:
	case Gl::COMPRESSED_SRGB8_ETC2:
		return PF_RGB_ETC2;
	case Gl::COMPRESSED_RGBA8_ETC2_EAC:
	case Gl::COMPRESSED_SRGB8_ALPHA8_ETC2_EAC:
		return PF_RGBA_ETC2;
	}

	return PF_NONE;
}

}
}
