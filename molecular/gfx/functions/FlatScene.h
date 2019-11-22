/*	FlatScene.h

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

#ifndef MOLECULAR_FLATSCENE_H
#define MOLECULAR_FLATSCENE_H

#include  <molecular/gfx/RenderFunction.h>

namespace molecular
{
namespace gfx
{

/// Uses brute-force frustum culling to call a collection of functions
class FlatScene : public RenderFunction
{
public:
	typedef RenderFunction Leaf;

	template<class TRenderManager>
	explicit FlatScene(TRenderManager& manager) : RenderFunction(manager) {}

	void Execute() override;
	util::AxisAlignedBox GetBounds() const override;
	bool BoundsChangedSince(int framecounter) const override;

	Leaf* Insert(RenderFunction* function) {mFunctions.insert(function); return function;}
	void Remove(Leaf* leaf) {mFunctions.erase(leaf);}

private:
	std::unordered_set<Leaf*> mFunctions;
};

}
}
#endif // MOLECULAR_FLATSCENE_H

