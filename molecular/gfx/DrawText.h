/*	DrawText.h

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

#ifndef MOLECULAR_DRAWTEXT_H
#define MOLECULAR_DRAWTEXT_H

#include <molecular/util/Vector3.h>
#include <molecular/util/Matrix4.h>
#include <vector>

#undef DrawText // Windows...

namespace molecular
{
namespace util
{
struct FontAtlasDescriptionFile;
}

namespace gfx
{
using namespace util;

/// Create vertex positions and texture coordinates for atlassed text
/** TString can be std::string or std::wstring. */
template<class TString>
void DrawText(
        const TString& text,
        const Vector3& start,
        float textHeight,
		const FontAtlasDescriptionFile& fontDescription,
        std::vector<Vector3>& outVertexPositions,
		std::vector<Vector2>& outVertexTexCoords
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
}

#endif // DRAWTEXT_H
