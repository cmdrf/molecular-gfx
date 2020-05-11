/*	Skeleton.cpp

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

#include "Skeleton.h"
#include <array>

namespace molecular
{
namespace gfx
{

void Skeleton::HandleExecute(Scope& scope)
{
	if(mActive)
	{
		auto& joints = scope.Bind<Uniform<Matrix4, CharacterAnimation::kBoneCount>>("bones"_H);
		for(int i = 0; i < CharacterAnimation::kBoneCount; i++)
			joints[i] = mJoints[i];
	}
	mCallee->Execute(&scope);
}

void Skeleton::SetJoints(const Matrix4 joints[CharacterAnimation::kBoneCount])
{
	mActive = true;
	std::array<Matrix4, CharacterAnimation::kBoneCount> absoluteTransforms;
	CharacterAnimation::SceneGraphToAbsoluteTransforms(joints, CharacterAnimation::kBoneParents, absoluteTransforms.data(), CharacterAnimation::kBoneCount);
	for(int i = 0; i < CharacterAnimation::kBoneCount; i++)
	{
		mJoints[i] = absoluteTransforms[i] * mBindPose[i];
	}
}

void Skeleton::SetBindPose(const Matrix4 inverseBindPoseMatrices[CharacterAnimation::kBoneCount])
{
	mActive = true;
	std::copy(inverseBindPoseMatrices, inverseBindPoseMatrices + CharacterAnimation::kBoneCount, mBindPose);
}

}
}
