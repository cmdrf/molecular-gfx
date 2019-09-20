/*	FlatScene.cpp

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

#include "FlatScene.h"
#include <molecular/util/Matrix4.h>
#include <molecular/util/Frustum.h>
#include <molecular/util/Plane.h>

namespace molecular
{
namespace Gfx
{

void FlatScene::Execute()
{
	const Uniform<Matrix4>* projMatrix = GetVariable<Uniform<Matrix4> >("projectionMatrix"_H);
	const Uniform<Matrix4>* viewMatrix = GetVariable<Uniform<Matrix4> >("viewMatrix"_H);

	if(!projMatrix || !viewMatrix)
		return;

	Matrix4 viewProjectionMatrix = **projMatrix * **viewMatrix;
	util::Frustum frustum(viewProjectionMatrix);

	for(auto func: mFunctions)
	{
		auto bounds = func->GetBounds(); // TODO: Cache
		if(frustum.Check(bounds) != util::Plane::kOutside)
			func->Execute();
	}
}

util::AxisAlignedBox FlatScene::GetBounds() const
{
	// TODO
	return util::AxisAlignedBox();
}

bool FlatScene::BoundsChangedSince(int framecounter) const
{
	// TODO
	return false;
}

}
}
