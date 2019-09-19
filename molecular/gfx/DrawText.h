/*	DrawText.h
	Copyright 2016-2017 Fabian Herb

	This file is part of Molecular Engine.
*/

#ifndef DRAWTEXT_H
#define DRAWTEXT_H

#include <molecular/util/Vector3.h>
#include <molecular/util/Matrix4.h>
#include <vector>

#undef DrawText // Windows...

namespace molecular
{

struct FontAtlasDescriptionFile;

/** TString can be std::string or std::wstring. */
template<class TString>
void DrawText(
        const TString& text,
        const Vector3& start,
        float textHeight,
        const FontAtlasDescriptionFile& fontDescription,
        std::vector<Vector3>& outVertexPositions,
        std::vector<Vector2>& out
        );

extern template void DrawText(
		const std::string& text,
		const Vector3& start,
		float textHeight,
		const FontAtlasDescriptionFile& fontDescription,
		std::vector<Vector3>& outVertexPositions,
		std::vector<Vector2>& outVertexTexCoords);
extern template void DrawText(
		const std::wstring& text,
		const Vector3& start,
		float textHeight,
		const FontAtlasDescriptionFile& fontDescription,
		std::vector<Vector3>& outVertexPositions,
		std::vector<Vector2>& outVertexTexCoords);

}

#endif // DRAWTEXT_H
