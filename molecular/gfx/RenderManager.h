/*	RenderManager.h

MIT License

Copyright (c) 2019-2020 Fabian Herb

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

#ifndef MOLECULAR_GFX_RENDERMANAGER_H
#define MOLECULAR_GFX_RENDERMANAGER_H

#include "RenderCmdSink.h"
#include "RenderFunction.h"
#include "MeshBoundsCollectionFile.h"

#include <molecular/gfx/Uniform.h>
#include <molecular/gfx/TextureManager.h>
#include <molecular/programgenerator/ProgramGenerator.h>
#include <molecular/gfx/ProgramProvider.h>
#include <molecular/gfx/MeshLoader.h>
#include <molecular/gfx/MaterialManager.h>
#include <molecular/util/ThreadSafeQueue.h>
#include <molecular/programgenerator/ProgramFile.h>
#include <molecular/gfx/DefaultProgramData.h>
#include <molecular/gfx/MaterialManager.h>
#include <molecular/util/ManualTaskQueue.h>
#include <molecular/util/MemoryStreamStorage.h>


namespace molecular
{
namespace gfx
{

class RenderContext;

/// Encloses all objects needed for rendering
template<class TFileServer, class TTaskQueue>
class RenderManagerT
{
public:
	using Self = RenderManagerT;
	using FileServer = TFileServer;
	using TaskQueue = TTaskQueue;
	using Renderer = gfx::RenderCmdSink;
	using RenderCmdSink = gfx::RenderCmdSink;
	using GlTaskQueue = ManualTaskQueue<typename TaskQueue::Mutex>;

	/// Constructor
	RenderManagerT(RenderContext& context, FileServer& fileServer, TaskQueue& queue, RenderCmdSink& commandSink);

	/// Main entry point for scene drawing
	void DrawOneFrame(RenderFunction& function, Scope& rootScope);

	/// Main entry point for scene drawing
	void DrawOneFrame(RenderFunction& function);

	/// Load a program definition file
	/** @see ProgramFile */
	bool LoadProgramFile(const Blob& fileContents);

	TaskQueue& GetTaskQueue() {return mTaskQueue;}
	GlTaskQueue& GetGlTaskQueue() {return mGlTaskQueue;}

	inline int GetFramecounter() const {return mFramecounter;}
	RenderContext& GetRenderContext() {return mRenderContext;}
	RenderCmdSink& GetRenderCmdSink() {return mRenderer;}

	FileServer& GetFileServer() {return mFileServer;}
	TextureManager& GetTextureManager() {return mTextureManager;}
	MeshManager& GetMeshManager() {return mMeshManager;}
	programgenerator::ProgramGenerator& GetProgramGenerator() {return mProgramGenerator;}
	ProgramProvider& GetProgramProvider() {return mProgramProvider;}
	MaterialManager& GetMaterialManager() {return mMaterialManager;}

	void SetMeshBoundsFileData(Blob&& fileData);
	const util::AxisAlignedBox& GetMeshFileBounds(Hash meshFile);

private:
	RenderContext& mRenderContext;
	RenderCmdSink& mRenderer;
	int mFramecounter;

	FileServer& mFileServer;

	GlTaskQueue mGlTaskQueue;
	TaskQueue& mTaskQueue;

	TextureLoader<Self> mTextureLoader;
	TextureManager mTextureManager;

	MeshLoader<Self> mMeshLoader;
	MeshManager mMeshManager;
	MaterialManager mMaterialManager;

	programgenerator::ProgramGenerator mProgramGenerator;
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
void RenderManagerT<TFileServer, TTaskQueue>::DrawOneFrame(RenderFunction& function, Scope& rootScope)
{
	mTextureManager.Update(mFramecounter);
	// Execute one task from the render thread queue
	mGlTaskQueue.RunOneTask();
	mFramecounter++;
	function.Execute(rootScope);
}

template<class TFileServer, class TTaskQueue>
void RenderManagerT<TFileServer, TTaskQueue>::DrawOneFrame(RenderFunction& function)
{
	Scope rootScope;
	DrawOneFrame(function, rootScope);
}

template<class TFileServer, class TTaskQueue>
bool RenderManagerT<TFileServer, TTaskQueue>::LoadProgramFile(const Blob& fileContents)
{
	try
	{
		char* fileBegin =  const_cast<char*>(static_cast<const char*>(fileContents.GetData()));
		programgenerator::ProgramFile progFile(fileBegin, fileBegin + fileContents.GetSize());

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
const util::AxisAlignedBox& RenderManagerT<TFileServer, TTaskQueue>::GetMeshFileBounds(Hash meshFile)
{
	if(mMeshBoundsCollectionFileData.GetData())
		return static_cast<const MeshBoundsCollectionFile*>(mMeshBoundsCollectionFileData.GetData())->GetBounds(meshFile);
	else
		return util::AxisAlignedBox::kDefault;
}

} // gfx
} // molecular

#endif // MOLECULAR_GFX_RENDERMANAGER_H
