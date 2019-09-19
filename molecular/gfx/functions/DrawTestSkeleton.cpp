/*	DrawTestSkeleton.cpp
	Copyright 2016 Fabian Herb

	This file is part of Molecular Engine.
*/

#include "DrawTestSkeleton.h"
#include <array>

namespace molecular
{
namespace Gfx
{

void DrawTestSkeleton::Execute()
{
	Binding<Attribute> position("vertexPositionAttr"_H, this);
	Binding<Uniform<Vector3>> color("diffuseColor"_H, this);

	*position = Attribute(mVertexBuffer, mPositionInfo);
	**color = Vector3(1, 0, 0);

	PrepareProgram();
	mRenderer.Draw(mIndexBuffer, mIndicesInfo);
}

void DrawTestSkeleton::SetJoints(const Matrix4 joints[CharacterAnimation::kBoneCount])
{
	std::array<Matrix4, CharacterAnimation::kBoneCount> absoluteTransforms;
	CharacterAnimation::SceneGraphToAbsoluteTransforms(joints, CharacterAnimation::kBoneParents, absoluteTransforms.data(), CharacterAnimation::kBoneCount);
	Vector3 positions[CharacterAnimation::kBoneCount];
	for(int i = 0; i < CharacterAnimation::kBoneCount; i++)
		positions[i] = absoluteTransforms[i].GetTranslation() * 0.01f; // TODO scaling
	mVertexBuffer->Store(positions, CharacterAnimation::kBoneCount * sizeof(Vector3), true);
}

}
}
