/*	Transform.cpp

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

#include "Transform.h"
#include <molecular/gfx/DefaultProgramData.h>
#include <molecular/util/Box.h>

namespace molecular
{
namespace gfx
{

void Transform::Execute()
{
	if(mCallee)
	{
		Binding<Uniform<Matrix4> > modelMatrix("modelMatrix"_H, this);

		**modelMatrix *= mTransform;
		mCallee->Execute();
	}
}

util::AxisAlignedBox Transform::GetBounds() const
{
	util::Box box(mCallee->GetBounds());
	box.Transform(mTransform);
	return box.ToAxisAligned();
}

bool Transform::BoundsChangedSince(int framecounter) const
{
	return mCallee && (mBoundsChangedFramecounter > framecounter || mCallee->BoundsChangedSince(framecounter));
}

}
}
