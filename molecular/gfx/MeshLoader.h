/*	MeshLoader.h
	Copyright 2012-2017 Fabian Herb

	This file is part of Molecular Engine.
*/

#ifndef MESHLOADER_H
#define MESHLOADER_H

#include "MeshManager.h"
#include <molecular/util/TaskDispatcher.h>
#include <molecular/gfx/functions/DrawMeshData.h>
#include <molecular/util/FileTypeIdentification.h>

namespace molecular
{
namespace Gfx
{

/// Loads mesh files
/** @todo LOD */
template<class TRenderManager>
class MeshLoader : public MeshManager::Loader
{
public:
	typedef TRenderManager RenderManager;

	MeshLoader(RenderManager& renderManager);

	DrawMeshData* Create() override;
	void Destroy(DrawMeshData*& asset) override;
	void StartLoad(MeshManager::Asset& asset, unsigned int minLevel, unsigned int maxLevel) override;
	void Unload(DrawMeshData*& asset, unsigned int minLevel, unsigned int maxLevel) override;

private:
	static void StoreMesh(MeshManager::Asset& destination, Blob& blob);

	static void StoreCompiledMesh(MeshManager::Asset& destination, Blob& blob);

	RenderManager& mRenderManager;
};

/*****************************************************************************/

template<class TRenderManager>
MeshLoader<TRenderManager>::MeshLoader(RenderManager& renderManager) :
	mRenderManager(renderManager)
{
}

template<class TRenderManager>
DrawMeshData* MeshLoader<TRenderManager>::Create()
{
	return new DrawMeshData(mRenderManager);
}

template<class TRenderManager>
void MeshLoader<TRenderManager>::Destroy(DrawMeshData*& asset)
{
	delete asset;
	asset = nullptr;
}

template<class TRenderManager>
void MeshLoader<TRenderManager>::StartLoad(MeshManager::Asset& asset, unsigned int minLevel, unsigned int maxLevel)
{
	// No LOD for now:
	assert(minLevel == 0);
	assert(maxLevel == 0);

	const Hash file = asset.GetLocation().meshFile;
	try
	{
		mRenderManager.GetFileServer().ReadFile(file, std::bind(StoreMesh, std::ref(asset), std::placeholders::_1), mRenderManager.GetGlTaskQueue());
	}
	catch(std::exception& e)
	{
		LOG(ERROR) << e.what();
		asset.SetState(0, MeshManager::Asset::kFailed);
	}
}

template<class TRenderManager>
void MeshLoader<TRenderManager>::Unload(DrawMeshData*& asset, unsigned int minLevel, unsigned int maxLevel)
{
	// No LOD for now:
	assert(minLevel == 0);
	assert(maxLevel == 0);

	asset->Unload();
}

template<class TRenderManager>
void MeshLoader<TRenderManager>::StoreMesh(MeshManager::Asset& destination, Blob& blob)
{
	if(FileTypeIdentification::IsCompiledMesh(blob.GetData(), blob.GetSize()))
		StoreCompiledMesh(destination, blob);
}

template<class TRenderManager>
void MeshLoader<TRenderManager>::StoreCompiledMesh(MeshManager::Asset& destination, Blob& blob)
{
	const MeshFile& file = *static_cast<const MeshFile*>(blob.GetData());
	try
	{
		destination.GetAsset()->Load(file);
		destination.SetState(0, MeshManager::Asset::kLoaded);
	}
	catch(std::exception& e)
	{
		LOG(ERROR) << "StoreCompiledMesh failed: " << e.what();
		destination.GetAsset()->Unload();
		destination.SetState(0, MeshManager::Asset::kFailed);
	}
}

}
}

#endif // MESHLOADER_H
