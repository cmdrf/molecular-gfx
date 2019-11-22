/*	DrawText.cpp

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

#include "DrawText.h"
#include <molecular/util/FontAtlasDescriptionFile.h>

namespace molecular
{
namespace gfx
{

static void DrawRect(
		float x, float y, float z,
		float w, float h,
		float s0, float t0,
		float s1, float t1,
		float scale,
		std::vector<Vector3>& outVertexPositions,
		std::vector<Vector2>& outVertexTexCoords)
{
	outVertexPositions.emplace_back(x * scale, y * scale, z);
	outVertexPositions.emplace_back(x * scale, (y - h) * scale, z);
	outVertexPositions.emplace_back((x + w) * scale, (y - h) * scale, z);
	outVertexPositions.emplace_back(x * scale, y * scale, z);
	outVertexPositions.emplace_back((x + w) * scale, (y - h) * scale, z);
	outVertexPositions.emplace_back((x + w) * scale, y* scale, z);

#if 1 // WTF?
	outVertexTexCoords.emplace_back(s0 - (s1 - s0) * 0.65f, t0);
	outVertexTexCoords.emplace_back(s0 - (s1 - s0) * 0.65f, t1);
	outVertexTexCoords.emplace_back(s0 + (s1 - s0) * 0.35f, t1);
	outVertexTexCoords.emplace_back(s0 - (s1 - s0) * 0.65f, t0);
	outVertexTexCoords.emplace_back(s0 + (s1 - s0) * 0.35f, t1);
	outVertexTexCoords.emplace_back(s0 + (s1 - s0) * 0.35f, t0);
#else
	outVertexTexCoords.emplace_back(s0, t0);
	outVertexTexCoords.emplace_back(s0, t1);
	outVertexTexCoords.emplace_back(s1, t1);
	outVertexTexCoords.emplace_back(s0, t0);
	outVertexTexCoords.emplace_back(s1, t1);
	outVertexTexCoords.emplace_back(s1, t0);
#endif
}

template<class TString>
void DrawText(
		const TString& text,
		const Vector3& start,
		float textHeight,
		const util::FontAtlasDescriptionFile& fontDescription,
		std::vector<Vector3>& outVertexPositions,
		std::vector<Vector2>& outVertexTexCoords
		)
{
	outVertexPositions.reserve(outVertexPositions.size() + text.size() * 6);
	outVertexTexCoords.reserve(outVertexTexCoords.size() + text.size() * 6);

	const float scale = textHeight / fontDescription.height * 1.33333f; // 96/72 ?
	float penX = start[0] / scale;
	float penY = start[1] / scale;

	typename TString::value_type previousChar = 0;
	int currentLine = 0;
	for(auto it = text.begin(); it != text.end(); ++it)
	{
		if(*it == L'\n')
		{
			currentLine++;
			penY -= textHeight;
			penX = start[0] / scale;
		}
		else
		{
			auto glyph = fontDescription.GetGlyph(*it);
			if(glyph)
			{
				penX += glyph->GetKerning(previousChar);
				float x = penX + glyph->offsetX;
				float y = penY + glyph->offsetY;
				DrawRect(x, y, start[2], glyph->width, glyph->height, glyph->s0, glyph->t0, glyph->s1, glyph->t1, scale, outVertexPositions, outVertexTexCoords);

				penX += glyph->advanceX;
				penY += glyph->advanceY;
				previousChar = *it;
			}
		}
	}
}

template void DrawText(
		const std::string& text,
		const Vector3& start,
		float textHeight,
		const util::FontAtlasDescriptionFile& fontDescription,
		std::vector<Vector3>& outVertexPositions,
		std::vector<Vector2>& outVertexTexCoords);
template void DrawText(
		const std::wstring& text,
		const Vector3& start,
		float textHeight,
		const util::FontAtlasDescriptionFile& fontDescription,
		std::vector<Vector3>& outVertexPositions,
		std::vector<Vector2>& outVertexTexCoords);

}
}
