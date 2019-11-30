/*	Skeleton.h

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

#ifndef MOLECULAR_SKELETON_H
#define MOLECULAR_SKELETON_H

#include <molecular/gfx/RenderFunction.h>
#include <molecular/util/CharacterAnimation.h>

namespace molecular
{
namespace gfx
{
using namespace util;

/// Set character animation uniforms
class Skeleton : public SingleCalleeRenderFunction
{
public:
	template<class TRenderManager>
	Skeleton(TRenderManager& manager);

	void Execute() override;
	util::AxisAlignedBox GetBounds() const override {return mCallee->GetBounds();}

	void SetJoints(const Matrix4 joints[CharacterAnimation::kBoneCount]);
	void SetBindPose(const Matrix4 inverseBindPoseMatrices[CharacterAnimation::kBoneCount]);

private:
	bool mActive = false;
	Matrix4 mJoints[CharacterAnimation::kBoneCount];
	Matrix4 mBindPose[CharacterAnimation::kBoneCount];
};

template<class TRenderManager>
Skeleton::Skeleton(TRenderManager& manager) :
    SingleCalleeRenderFunction(manager)
{

}

}
}

#endif // MOLECULAR_SKELETON_H
