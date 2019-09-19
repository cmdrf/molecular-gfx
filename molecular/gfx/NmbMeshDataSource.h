#ifndef NMBMESHDATASOURCE_H
#define NMBMESHDATASOURCE_H

#include "gfx/MeshDataSource.h"
#include "util/NmbFile.h"

class NmbMeshDataSource : public MeshDataSource
{
public:
	NmbMeshDataSource(NmbFile& file, const std::string &submesh = "");

	int PrepareVertexData(LayoutHint layout = kLayoutAny);

	int PrepareIndexData();

	unsigned int VertexBufferSize(int buffer);
	unsigned int IndexBufferSize(int buffer);
	const void* VertexBufferData(int buffer);
	const void* IndexBufferData(int buffer);
	AxisAlignedBox GetBounds() const;

private:
	NmbFile& mFile;
	std::string mSubmesh;
	std::vector<const NmbFile::VertexBuffer*> mVertexBuffers;
	std::vector<const NmbFile::IndexBuffer*> mIndexBuffers;
};

#endif // NMBMESHDATASOURCE_H
