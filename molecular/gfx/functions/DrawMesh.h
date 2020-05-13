/*	DrawMesh.h

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

#ifndef MOLECULAR_DRAWMESH_H
#define MOLECULAR_DRAWMESH_H

#include <molecular/gfx/RenderFunction.h>
#include <molecular/gfx/MeshManager.h>
#include "DrawMeshData.h"

namespace molecular
{
namespace gfx
{

/// Draws a mesh by specifying a mesh filename
/** Meshes (in the form of DrawMeshData objects) are loaded on demand. */
template<class TRenderManager>
class DrawMesh : public RenderFunction
{
public:
	using RenderManager = TRenderManager;

	explicit DrawMesh(RenderManager& renderManager) :
		RenderFunction(renderManager),
		mRenderManager(renderManager)
	{

	}

	util::AxisAlignedBox GetBounds() const override {return mBounds;}
	bool BoundsChangedSince(int framecounter) const override {return mLastBoundsChange > framecounter;}

	void SetMeshFile(Hash mesh);
	void AddMorphTarget(const std::string& targetFile, float weight = 1.0);
	void ClearMorphTargets();
	void SetPickingId(unsigned int id) {mPickingId = id;}

protected:
	void HandleExecute(Scope& scope) override;

private:
	void DataChanged();

	MeshLocator mLocator;
	MeshManager::Asset* mAsset = nullptr;
	util::AxisAlignedBox mBounds;
	int mLastBoundsChange = 0;
	unsigned int mPickingId = 0;
	RenderManager& mRenderManager;
};

/******************************************************************************/

template<class TRenderManager>
void DrawMesh<TRenderManager>::HandleExecute(Scope& scope)
{
	if(!mAsset && mLocator.meshFile != ""_H)
	{
		mAsset = mRenderManager.GetMeshManager().GetAsset(mLocator);
	}

	if(mAsset)
	{
		DrawMeshData* data = mAsset->Use();
		if(mBounds.IsNull())
		{
			mBounds = data->GetBounds();
//			mLastBoundsChange = mRenderManager.GetFramecounter();
		}
		scope.Set("pickingColor"_H, Uniform<unsigned int>(mPickingId));
		data->Execute(scope);
	}
}

template<class TRenderManager>
void DrawMesh<TRenderManager>::SetMeshFile(Hash mesh)
{
	mAsset = nullptr;

	mLocator.meshFile = mesh;
	mBounds = mRenderManager.GetMeshFileBounds(mesh);
	mLastBoundsChange = mRenderManager.GetFramecounter();
}

template<class TRenderManager>
void DrawMesh<TRenderManager>::AddMorphTarget(const std::string& targetFile, float weight)
{
	mAsset = nullptr;

	MeshLocator::MorphTarget target;
	target.file = targetFile;
	target.weight = weight;
	mLocator.morphTargets.push_back(target);
}

template<class TRenderManager>
void DrawMesh<TRenderManager>::ClearMorphTargets()
{
	mAsset = nullptr;
	mLocator.morphTargets.clear();
}

}
}

#endif // MOLECULAR_DRAWMESH_H

