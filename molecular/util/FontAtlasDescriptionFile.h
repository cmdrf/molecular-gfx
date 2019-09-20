/*	FontAtlasDesciptionFile.h

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
SOFTWARE.
*/

#ifndef MOLECULAR_FONTATLASDESCRIPTIONFILE_H
#define MOLECULAR_FONTATLASDESCRIPTIONFILE_H

#include <stdint.h>

namespace molecular
{

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

}

#endif // MOLECULAR_FONTATLASDESCRIPTIONFILE_H
