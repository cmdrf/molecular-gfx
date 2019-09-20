/*	DrawMeshData.h

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

#ifndef MOLECULAR_DRAWMESHDATA_H
#define MOLECULAR_DRAWMESHDATA_H

#include "DrawingFunction.h"
#include <molecular/gfx/ProgramProvider.h>
#include <molecular/gfx/Material.h>
#include <molecular/gfx/MaterialManager.h>


namespace molecular
{
struct MeshFile;
class MeshDataSource;

namespace Gfx
{

/// RenderFunction that draws a group of meshes
/** @todo LOD */
class DrawMeshData : public DrawingFunction
{
public:
	/// Construct and load data
	template<class TRenderManager>
	DrawMeshData(TRenderManager& manager, MeshDataSource& source) :
		DrawingFunction(manager),
		mMaterialManager(manager.GetMaterialManager())
	{
		Load(source);
	}

	/// Construct without loading data
	/** Data must be loaded later with Load(). */
	template<class TRenderManager>
	DrawMeshData(TRenderManager& manager) :
		DrawingFunction(manager),
		mMaterialManager(manager.GetMaterialManager())
	{

	}

	~DrawMeshData() override;

	/** Draws meshes if they are loaded. Does nothing otherwise. */
	void Execute() override;

	util::AxisAlignedBox GetBounds() const override {return mBounds;}

	/// Store mesh data in Renderer
	/** Called by MeshLoader::StoreNmbMesh, MeshLoader::StoreObjMesh or
		MeshLoader::StoreXmlMesh.
		@deprecated Load compiled meshes! */
	void Load(MeshDataSource& source);

	/// Store mesh data in Renderer
	/** Called by MeshLoader::StoreCompiledMesh. */
	void Load(const MeshFile& file);

	void Unload();

private:
	struct VertexDataSet
	{
		std::vector<VertexAttributeInfo> attributes;
	};

	struct Mesh
	{
		/// Index buffer info
		/** References one entry from mVertexDataSets. */
		IndexBufferInfo info;
		Material* material;
	};

	void Draw(Mesh& mesh);

	/// Binds alls attributes and calls Draw
	void BindAttributesAndDraw(Mesh& mesh, std::vector<VertexAttributeInfo>::iterator begin, std::vector<VertexAttributeInfo>::iterator end);

	MaterialManager& mMaterialManager;

	/// List of vertex data sets
	/** Can reference one or more entries from mVertexBuffers */
	std::vector<VertexDataSet> mVertexDataSets;

	/// Handles to vertex data on GPU
	std::vector<RenderCmdSink::VertexBuffer*> mVertexBuffers;

	/// Handles to index data on GPU
	std::vector<RenderCmdSink::IndexBuffer*> mIndexBuffers;

	std::vector<Mesh> mMeshes;
	util::AxisAlignedBox mBounds;
};

}
}

#endif // MOLECULAR_DRAWMESH_H
