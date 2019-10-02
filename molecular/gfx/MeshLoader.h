/*	MeshLoader.h

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

#ifndef MOLECULAR_MESHLOADER_H
#define MOLECULAR_MESHLOADER_H

#include "MeshManager.h"

#include <molecular/gfx/functions/DrawMeshData.h>
#include <molecular/gfx/NmbMeshDataSource.h>

#include <molecular/util/FileTypeIdentification.h>
#include <molecular/util/MemoryStreamStorage.h>
#include <molecular/util/NmbFile.h>
#include <molecular/util/TaskDispatcher.h>

namespace molecular
{
namespace gfx
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
	static void StoreNmb(MeshManager::Asset& destination, Blob& blob);

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
	else if(FileTypeIdentification::IsNmb(blob.GetData(), blob.GetSize()))
		StoreNmb(destination, blob);
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


template<class TRenderManager>
void MeshLoader<TRenderManager>::StoreNmb(MeshManager::Asset& destination, Blob& blob)
{
	MemoryReadStorage storage(blob.GetData(), blob.GetSize());
	try
	{
		NmbFile nmb(storage);
		NmbMeshDataSource source(nmb);
		destination.GetAsset()->Load(source);
		destination.SetState(0, MeshManager::Asset::kLoaded);
	}
	catch(std::exception& e)
	{
		LOG(ERROR) << "StoreNmb failed: " << e.what();
		destination.GetAsset()->Unload();
		destination.SetState(0, MeshManager::Asset::kFailed);
	}
}

}
}

#endif // MOLECULAR_MESHLOADER_H
