/*	DrawMesh.h
	Copyright 2015-2017 Fabian Herb

	This file is part of Molecular Engine.
*/

#ifndef DRAWMESH_H
#define DRAWMESH_H

#include "gfx/RenderFunction.h"
#include "gfx/MeshManager.h"
#include "DrawMeshData.h"

namespace Gfx
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

	void Execute() override;
	AxisAlignedBox GetBounds() const override {return mBounds;}
	bool BoundsChangedSince(int framecounter) const override {return mLastBoundsChange > framecounter;}

	void SetMeshFile(Hash mesh);
	void AddMorphTarget(const std::string& targetFile, float weight = 1.0);
	void ClearMorphTargets();
	void SetPickingId(unsigned int id) {mPickingId = id;}

private:
	void DataChanged();

	MeshLocator mLocator;
	MeshManager::Asset* mAsset = nullptr;
	AxisAlignedBox mBounds;
	int mLastBoundsChange = 0;
	unsigned int mPickingId = 0;
	RenderManager& mRenderManager;
};

/******************************************************************************/

template<class TRenderManager>
void DrawMesh<TRenderManager>::Execute()
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
		Binding<Uniform<unsigned int> > pickingColor("pickingColor"_H, this);
		**pickingColor = mPickingId;
		data->Execute();
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

#endif // DRAWMESH_H

