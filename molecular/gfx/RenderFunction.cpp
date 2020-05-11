/*	RenderFunction.cpp

MIT License

Copyright (c) 2020 Fabian Herb

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

#include "RenderFunction.h"

namespace molecular
{
namespace gfx
{

void RenderFunction::Execute(const Scope* parentScope)
{
	if(mVisible)
	{
		Scope scope(parentScope);
		HandleExecute(scope);
	}
}

AxisAlignedBox MultipleCalleeRenderFunction::GetBounds() const
{
	// Create bounding box that encloses all boxes of the callees:
	AxisAlignedBox box(0, 0, 0, 0, 0, 0);
	for(RenderFunction* callee: mCallees)
	{
		box.Stretch(callee->GetBounds());
	}
	return box;
}

}
}
