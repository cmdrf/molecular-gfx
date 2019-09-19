/*	DrawText.cpp
	Copyright 2016 Fabian Herb

	This file is part of Molecular Engine.
*/

#include "DrawText.h"
#include <molecular/util/FontAtlasDescriptionFile.h>

namespace molecular
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
		const FontAtlasDescriptionFile& fontDescription,
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
		const FontAtlasDescriptionFile& fontDescription,
		std::vector<Vector3>& outVertexPositions,
		std::vector<Vector2>& outVertexTexCoords);
template void DrawText(
		const std::wstring& text,
		const Vector3& start,
		float textHeight,
		const FontAtlasDescriptionFile& fontDescription,
		std::vector<Vector3>& outVertexPositions,
		std::vector<Vector2>& outVertexTexCoords);

}
