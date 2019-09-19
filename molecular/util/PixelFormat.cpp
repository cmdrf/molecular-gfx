/*	PixelFormat.cpp
	Copyright 2015 Fabian Herb

	This file is part of Molecular Engine.
*/

#include "PixelFormat.h"

namespace Pf
{

	bool IsCompressed(PixelFormat format)
	{
		switch (format)
		{
		case PF_RGB_DXT1:
		case PF_RGBA_DXT1:
		case PF_RGBA_DXT3:
		case PF_RGBA_DXT5:
		case PF_RGB_ETC1:
		case PF_RGB_ETC2:
		case PF_RGBA_ETC2:
		case PF_RGB_DXT1_LINEAR:
		case PF_RGBA_DXT1_LINEAR:
		case PF_RGBA_DXT3_LINEAR:
		case PF_RGBA_DXT5_LINEAR:
		case PF_RGB_ETC1_LINEAR:
		case PF_RGB_ETC2_LINEAR:
		case PF_RGBA_ETC2_LINEAR:
			return true;
		default:
			return false;
		}
	}

}

std::ostream& operator<<(std::ostream& o, PixelFormat format)
{
	switch (format)
	{
	case PF_NONE: o << "NONE"; break;
	case PF_UNSUPPORTED: o << "UNSUPPORTED"; break;
	case PF_ANY: o << "ANY"; break;
	case PF_ANY_RGB: o << "PF_ANY_RGB"; break;
	case PF_ANY_RGBA: o << "PF_ANY_RGBA"; break;
	case PF_R8G8B8: o << "PF_R8G8B8"; break;
	case PF_R8G8B8A8: o << "PF_R8G8B8A8"; break;
	case PF_B8G8R8: o << "PF_B8G8R8"; break;
	case PF_B8G8R8A8: o << "PF_B8G8R8A8"; break;
	case PF_R4G4B4A4: o << "PF_R4G4B4A4"; break;
	case PF_R5G5B5A1: o << "PF_R5G5B5A1"; break;
	case PF_L8: o << "PF_L8"; break;
	case PF_L32_UNSIGNED: o << "PF_L32_UNSIGNED"; break;
	case PF_L8A8: o << "PF_L8A8"; break;
	case PF_A8: o << "PF_A8"; break;
	case PF_R8G8B8_SIGNED: o << "PF_R8G8B8_SIGNED"; break;
	case PF_R8G8B8_SIGNED_NORM: o << "PF_R8G8B8_SIGNED_NORM"; break;
	case PF_R8G8B8A8_UNSIGNED: o << "PF_R8G8B8A8_UNSIGNED"; break;
	case PF_L_FLOAT32: o << "PF_L_FLOAT32"; break;
	case PF_RGB_FLOAT16: o << "PF_RGB_FLOAT16"; break;
	case PF_RGB_FLOAT32: o << "PF_RGB_FLOAT32"; break;
	case PF_RGBA_FLOAT16: o << "PF_RGBA_FLOAT16"; break;
	case PF_RGBA_FLOAT32: o << "PF_RGBA_FLOAT32"; break;
	case PF_DEPTH_16: o << "PF_DEPTH_16"; break;
	case PF_DEPTH_24: o << "PF_DEPTH_24"; break;
	case PF_DEPTH_32: o << "PF_DEPTH_32"; break;
	case PF_DEPTH_FLOAT32: o << "PF_DEPTH_FLOAT32"; break;
	case PF_STENCIL_8: o << "PF_STENCIL_8"; break;
	case PF_RGB_DXT1: o << "PF_RGB_DXT1"; break;
	case PF_RGBA_DXT1: o << "PF_RGBA_DXT1"; break;
	case PF_RGBA_DXT3: o << "PF_RGBA_DXT3"; break;
	case PF_RGBA_DXT5: o << "PF_RGBA_DXT5"; break;
	case PF_RGB_ETC1: o << "PF_RGB_ETC1"; break;
	case PF_RGB_ETC2: o << "PF_RGB_ETC2"; break;
	case PF_RGBA_ETC2: o << "PF_RGBA_ETC2"; break;
	default: o << "Unknown"; break;
	}
	return o;
}
