/*	RenderManager.h
	Copyright 2012-2017 Fabian Herb

	This file is part of Molecular Engine.
*/

#ifndef RENDERMANAGER_H
#define RENDERMANAGER_H

#include "util/DynamicScoping.h"
#include "RenderCmdSink.h"
#include "gfx/Uniform.h"
#include "gfx/TextureManager.h"
#include "gfx/ProgramGenerator.h"
#include "gfx/ProgramProvider.h"
#include "gfx/MeshLoader.h"
#include "gfx/MaterialManager.h"
#include "util/ThreadSafeQueue.h"
#include "Scoping.h"
#include "RenderFunction.h"
#include "ProgramFile.h"
#include "gfx/DefaultProgramData.h"
#include "gfx/MaterialManager.h"
#include "util/ManualTaskQueue.h"
#include "util/MemoryStreamStorage.h"
#include "MeshBoundsCollectionFile.h"

class RenderContext;

namespace Gfx
{

/// Encloses all objects needed for rendering
template<class TFileServer, class TTaskQueue>
class RenderManagerT
{
public:
	typedef RenderManagerT Self;
	typedef TFileServer FileServer;
	using TaskQueue = TTaskQueue;
	using Renderer = Gfx::RenderCmdSink;
	using RenderCmdSink = Gfx::RenderCmdSink;
	typedef Gfx::TextureManager TextureManager;
	typedef ManualTaskQueue<typename TaskQueue::Mutex> GlTaskQueue;

	/// Constructor
	RenderManagerT(RenderContext& context, FileServer& fileServer, TaskQueue& queue, RenderCmdSink& commandSink);

	/// Main entry point for scene drawing
	void DrawOneFrame(RenderFunction& function);

	/// Load a program definition file
	/** @see ProgramFile */
	bool LoadProgramFile(const Blob& fileContents);

	TaskQueue& GetTaskQueue() {return mTaskQueue;}
	GlTaskQueue& GetGlTaskQueue() {return mGlTaskQueue;}

	inline int GetFramecounter() const {return mFramecounter;}
	Gfx::Scoping& GetScoping() {return mScoping;}
	RenderContext& GetRenderContext() {return mRenderContext;}
	RenderCmdSink& GetRenderCmdSink() {return mRenderer;}

	FileServer& GetFileServer() {return mFileServer;}
	TextureManager& GetTextureManager() {return mTextureManager;}
	MeshManager& GetMeshManager() {return mMeshManager;}
	ProgramGenerator& GetProgramGenerator() {return mProgramGenerator;}
	ProgramProvider& GetProgramProvider() {return mProgramProvider;}
	MaterialManager& GetMaterialManager() {return mMaterialManager;}

	void SetMeshBoundsFileData(Blob&& fileData);
	const AxisAlignedBox& GetMeshFileBounds(Hash meshFile);

private:
	RenderContext& mRenderContext;
	RenderCmdSink& mRenderer;
	int mFramecounter;
	Gfx::Scoping mScoping;

	FileServer& mFileServer;

	GlTaskQueue mGlTaskQueue;
	TaskQueue& mTaskQueue;

	TextureLoader<Self> mTextureLoader;
	TextureManager mTextureManager;

	MeshLoader<Self> mMeshLoader;
	MeshManager mMeshManager;
	MaterialManager mMaterialManager;

	ProgramGenerator mProgramGenerator;
	ProgramProvider mProgramProvider;

	Blob mMeshBoundsCollectionFileData;
};

template<class TFileServer, class TTaskQueue>
RenderManagerT<TFileServer, TTaskQueue>::RenderManagerT(RenderContext& context, FileServer& fileServer, TaskQueue& queue, RenderCmdSink& commandSink) :
	mRenderContext(context),
	mRenderer(commandSink),
	mFramecounter(0),
	mFileServer(fileServer),
	mTaskQueue(queue),
	mTextureLoader(*this),
	mTextureManager(mTextureLoader),
	mMeshLoader(*this),
	mMeshManager(mMeshLoader),
	mMaterialManager(*this),
	mProgramProvider(mRenderer, mProgramGenerator)
{
	mRenderer.Init();
	DefaultProgramData::FeedToGenerator(mProgramGenerator);
}

template<class TFileServer, class TTaskQueue>
void RenderManagerT<TFileServer, TTaskQueue>::DrawOneFrame(RenderFunction& function)
{
	mTextureManager.Update(mFramecounter);
	// Execute one task from the render thread queue
	mGlTaskQueue.RunOneTask();
	mFramecounter++;
	function.Execute();
}

template<class TFileServer, class TTaskQueue>
bool RenderManagerT<TFileServer, TTaskQueue>::LoadProgramFile(const Blob& fileContents)
{
	try
	{
		MemoryReadStorage storage(fileContents.GetData(), fileContents.GetSize());
		ProgramFile progFile(storage);

		for(auto& it: progFile.GetVariables())
		{
			mProgramGenerator.AddVariable(it);
		}

		for(auto& it2: progFile.GetFunctions())
		{
			mProgramGenerator.AddFunction(it2);
		}
	}
	catch(const std::exception& e)
	{
		LOG(ERROR) << "Error loading program file: " << e.what();
		return false;
	}

	return true;
}

template<class TFileServer, class TTaskQueue>
void RenderManagerT<TFileServer, TTaskQueue>::SetMeshBoundsFileData(Blob&& fileData)
{
	mMeshBoundsCollectionFileData = std::move(fileData);
}

template<class TFileServer, class TTaskQueue>
const AxisAlignedBox& RenderManagerT<TFileServer, TTaskQueue>::GetMeshFileBounds(Hash meshFile)
{
	assert(mMeshBoundsCollectionFileData.GetData());
	return static_cast<const MeshBoundsCollectionFile*>(mMeshBoundsCollectionFileData.GetData())->GetBounds(meshFile);
}

}

#endif // RENDERMANAGER_H
