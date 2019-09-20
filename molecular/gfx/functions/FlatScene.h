/*	FlatScene.h
	Copyright 2014-2017 Fabian Herb

	This file is part of Molecular Engine.
*/

#ifndef FLATSCENE_H
#define FLATSCENE_H

#include  <molecular/gfx/RenderFunction.h>

namespace molecular
{
namespace Gfx
{

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
#endif // FLATSCENE_H

