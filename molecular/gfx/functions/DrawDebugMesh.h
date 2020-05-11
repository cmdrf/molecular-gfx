/*	DrawDebugMesh.h

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

#ifndef MOLECULAR_DRAWDEBUGMESH_H
#define MOLECULAR_DRAWDEBUGMESH_H

#include "DrawingFunction.h"
#include <molecular/gfx/DrawText.h>
#include <molecular/gfx/TextureManager.h>
#include <molecular/util/Blob.h>
#include <molecular/util/FontAtlasDescriptionFile.h>

namespace molecular
{
namespace gfx
{

/// Draws lines and text
/** Buffers are erased after each Execute().
	@todo Text orientation. */
//template<class TRenderManager>
class DrawDebugMesh : public DrawingFunction
{
public:
//	using RenderManager = TRenderManager;
//	using Renderer = typename RenderManager::Renderer;

	template<class TRenderManager>
	DrawDebugMesh(TRenderManager& renderManager);

	~DrawDebugMesh() override;

	util::AxisAlignedBox GetBounds() const override {return mBounds;}

	void DrawLine(const Vector3& from, const Vector3& to, const Vector3& color);
	void DrawText(const std::string& text, const Vector3& position);
	void DrawAxisAlignedBox(const Vector3& min, const Vector3& max, const Vector3& color);
	void DrawCoordSystem(const Matrix4& coordSystem);

protected:
	void HandleExecute(Scope& scope) override;

private:
	std::vector<Vector3> mLineVertexData;
	std::vector<Vector3> mLineColorData;
	std::vector<Vector3> mTextVertexData;
	std::vector<Vector2> mTextTexCoordData;
	typename RenderCmdSink::VertexBuffer* mLinePositionBuffer;
	typename RenderCmdSink::VertexBuffer* mLineColorBuffer;
	typename RenderCmdSink::VertexBuffer* mTextVertexBuffer;
	typename RenderCmdSink::VertexBuffer* mTextTexCoordBuffer;
	VertexAttributeInfo mVector3Info;
	VertexAttributeInfo mVector2Info;
	util::AxisAlignedBox mBounds;

	/** @todo Move to some central place */
	Blob mFontDescriptionData;

	TextureManager::Asset* mTextTextureHandle = nullptr;
};

/******************************************************************************/

template<class TRenderManager>
DrawDebugMesh::DrawDebugMesh(TRenderManager& renderManager) :
    DrawingFunction(renderManager)
{
	mVector3Info.components = 3;
	mVector3Info.type = VertexAttributeInfo::kFloat;
	mVector2Info.components = 2;
	mVector2Info.type = VertexAttributeInfo::kFloat;

	mLinePositionBuffer = renderManager.GetRenderCmdSink().CreateVertexBuffer();
	mLineColorBuffer = renderManager.GetRenderCmdSink().CreateVertexBuffer();
	mTextVertexBuffer = renderManager.GetRenderCmdSink().CreateVertexBuffer();
	mTextTexCoordBuffer = renderManager.GetRenderCmdSink().CreateVertexBuffer();

	auto handler = [this](Blob& blob){mFontDescriptionData = std::move(blob);};
	renderManager.GetFileServer().ReadFile("/common/Vera.mfi"_H, handler, renderManager.GetGlTaskQueue());
	mTextTextureHandle = renderManager.GetTextureManager().GetAsset("/common/Vera.tga"_H);
}

inline DrawDebugMesh::~DrawDebugMesh()
{
	mRenderer.DestroyVertexBuffer(mLinePositionBuffer);
	mRenderer.DestroyVertexBuffer(mLineColorBuffer);
	mRenderer.DestroyVertexBuffer(mTextVertexBuffer);
	mRenderer.DestroyVertexBuffer(mTextTexCoordBuffer);
}

//template<class TRenderManager>
inline void DrawDebugMesh::HandleExecute(Scope& scope)
{
	mLinePositionBuffer->Store(mLineVertexData.data(), mLineVertexData.size() * sizeof(Vector3));
	mLineColorBuffer->Store(mLineColorData.data(), mLineColorData.size() * sizeof(Vector3));
	mTextVertexBuffer->Store(mTextVertexData.data(), mTextVertexData.size() * sizeof(Vector3));
	mTextTexCoordBuffer->Store(mTextTexCoordData.data(), mTextTexCoordData.size() * sizeof(Vector2));

	{
		Scope linesScope(&scope);
		linesScope.Set("vertexPositionAttr"_H, Attribute(mLinePositionBuffer, mVector3Info));
		linesScope.Set("vertexColorAttr"_H, Attribute(mLineColorBuffer, mVector3Info));

		PrepareProgram(linesScope);
		mRenderer.Draw(IndexBufferInfo::Mode::kLines, mLineVertexData.size());
	}
	{
		Scope textScope(&scope);
		textScope.Set("vertexPositionAttr"_H, Attribute(mTextVertexBuffer, mVector3Info));
		textScope.Set("vertexUv0Attr"_H, Attribute(mTextTexCoordBuffer, mVector2Info));
		textScope.Set("diffuseColor"_H, Uniform<Vector3>(Vector3(1, 1, 1)));
		textScope.Set("signedDistanceFieldTexture"_H, Uniform<RenderCmdSink::Texture*>(mTextTextureHandle->Use()));

		PrepareProgram(textScope);
		mRenderer.SetBlending(true, RenderCmdSink::kSrcAlpha, RenderCmdSink::kOneMinusSrcAlpha);
		mRenderer.Draw(IndexBufferInfo::Mode::kTriangles, mTextVertexData.size());
		mRenderer.SetBlending(false);
	}

	mLineVertexData.clear();
	mLineColorData.clear();
	mTextVertexData.clear();
	mTextTexCoordData.clear();
	mBounds = util::AxisAlignedBox();
}

//template<class TRenderManager>
inline void DrawDebugMesh::DrawLine(const Vector3& from, const Vector3& to, const Vector3& color)
{
	mLineVertexData.push_back(from);
	mLineVertexData.push_back(to);
	mLineColorData.push_back(color);
	mLineColorData.push_back(color);
	mBounds.Stretch(from);
	mBounds.Stretch(to);
}

inline void DrawDebugMesh::DrawText(const std::string& text, const Vector3& position)
{
	if(mFontDescriptionData.GetSize() > 0)
	{
		auto desc = static_cast<const util::FontAtlasDescriptionFile*>(mFontDescriptionData.GetData());
		gfx::DrawText(text, position, 0.1f, *desc, mTextVertexData, mTextTexCoordData);
		mBounds.Stretch(position);
	}
}

inline void DrawDebugMesh::DrawAxisAlignedBox(const Vector3& min, const Vector3& max, const Vector3& color)
{
	const Vector3& p0 = min;
	Vector3 p1(min[0], min[1], max[2]);
	Vector3 p2(min[0], max[1], min[2]);
	Vector3 p3(min[0], max[1], max[2]);
	Vector3 p4(max[0], min[1], min[2]);
	Vector3 p5(max[0], min[1], max[2]);
	Vector3 p6(max[0], max[1], min[2]);
	const Vector3& p7 = max;
	DrawLine(p0, p1, color);
	DrawLine(p0, p2, color);
	DrawLine(p0, p4, color);
	DrawLine(p1, p3, color);
	DrawLine(p1, p5, color);
	DrawLine(p2, p3, color);
	DrawLine(p2, p6, color);
	DrawLine(p3, p7, color);
	DrawLine(p4, p5, color);
	DrawLine(p4, p6, color);
	DrawLine(p5, p7, color);
	DrawLine(p6, p7, color);
}

inline void DrawDebugMesh::DrawCoordSystem(const Matrix4& coordSystem)
{
	Vector3 x(coordSystem.ToEigen().row(0).data());
	Vector3 y(coordSystem.ToEigen().row(1).data());
	Vector3 z(coordSystem.ToEigen().row(2).data());
	Vector3 p0 = coordSystem.GetTranslation();
	DrawLine(p0, p0 + x, Vector3(1, 0, 0));
	DrawLine(p0, p0 + y, Vector3(0, 1, 0));
	DrawLine(p0, p0 + z, Vector3(0, 0, 1));
}

}
}

#endif // MOLECULAR_DRAWDEBUGMESH_H
