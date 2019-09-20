/*	DrawTestSkeleton.cpp

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

#include "DrawTestSkeleton.h"
#include <array>

namespace molecular
{
namespace gfx
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
