/*	FontAtlasDesciptionFile.h
	Copyright 2016 Fabian Herb

	This file is part of Molecular Engine.
*/

#ifndef FONTATLASDESCRIPTIONFILE_H
#define FONTATLASDESCRIPTIONFILE_H

#include <stdint.h>

struct FontAtlasDescriptionFile
{
	struct UtfOffset
	{
		int32_t charcode = 0;
		uint32_t offset = 0;
	};

	struct KerningInfo
	{
		int32_t charcode = 0;
		float kerning = 0;
	};

	struct GlyphInfo
	{
//		uint32_t charcode;
		uint32_t width;
		uint32_t height;
		int32_t offsetX;
		int32_t offsetY;
		float advanceX;
		float advanceY;
		float s0;
		float t0;
		float s1;
		float t1;
		uint32_t kerningCount;
		KerningInfo kerningInfos[0];

		float GetKerning(wchar_t charcode) const
		{
			for(unsigned int i = 0; i < kerningCount; ++i)
			{
				if(kerningInfos[i].charcode == charcode)
					return kerningInfos[i].kerning;
			}
			return 0;
		}
	};

	static const uint32_t kMagic = 0xf061f11f;
	uint32_t magic;
	uint32_t fontTextureFile;
	float size;
	float height;
	float linegap;
	float ascender;
	float descender;
	uint32_t asciiOffsets[128];
	uint32_t glyphCount;
	UtfOffset utfOffsets[0];

	const GlyphInfo* GetGlyph(wchar_t charcode) const
	{
		if(charcode < 128)
			return GetGlyph(static_cast<char>(charcode));
		else
		{
			for(unsigned int i = 0; i < glyphCount; ++i)
			{
				if(utfOffsets[i].charcode == charcode)
					return reinterpret_cast<const GlyphInfo*>(reinterpret_cast<const uint8_t*>(this) + utfOffsets[i].offset);
			}
		}
		return nullptr;
	}

	const GlyphInfo* GetGlyph(char charcode) const
	{
		if(asciiOffsets[charcode] < sizeof(FontAtlasDescriptionFile))
			return nullptr;
		return reinterpret_cast<const GlyphInfo*>(reinterpret_cast<const uint8_t*>(this) + asciiOffsets[charcode]);
	}
};

#endif // FONTATLASDESCRIPTIONFILE_H
