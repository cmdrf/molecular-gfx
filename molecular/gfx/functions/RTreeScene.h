/*	RTreeScene.h

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

#ifndef MOLECULAR_RTREESCENE_H
#define MOLECULAR_RTREESCENE_H

#include <molecular/gfx/RenderFunction.h>
#include <molecular/gfx/RenderManager.h>
#include <molecular/util/RTree.h>
#include <molecular/util/Frustum.h>

namespace molecular
{
class Matrix4;

namespace gfx
{

/** @bug Frustum visibility check does not work and is commented out. */
class RTreeScene : public RenderFunction
{
private:
	struct LeafData
	{
		int boundsUpdated;
		RenderFunction* function;
	};

	typedef util::RTree<LeafData> Tree;

public:
	typedef Tree::Leaf Leaf;

	RTreeScene(RenderManager& manager);

	void Execute() override;
	util::AxisAlignedBox GetBounds() const override;
	bool BoundsChangedSince(int framecounter) const override;


	Leaf* Insert(RenderFunction* function);
	void Remove(Leaf* leaf);

private:
	void Draw(LeafData& data);
	void Draw(const Matrix4& viewProjectionMatrix);

	/// Acceptor for RTree::Query that accepts everything
	class AllAcceptor
	{
	public:
		Tree::AcceptorOutput operator() (const util::AxisAlignedBox& box)
		{
			return Tree::kAccept;
		}
	};

	class FrustumAcceptor
	{
	public:
		FrustumAcceptor(const util::Frustum& frustum) : mFrustum(frustum) {}
		Tree::AcceptorOutput operator() (const util::AxisAlignedBox& box) const
		{
			util::Plane::IntersectStatus result = mFrustum.Check(box);
			if(result == util::Plane::kInside)
				return Tree::kAccept;
			else if(result == util::Plane::kOutside)
				return Tree::kDiscard;
			else
				return Tree::kDescend;
		}

	private:
		const util::Frustum& mFrustum;
	};

	class UpdateVisitor : public Tree::Visitor
	{
	public:
		UpdateVisitor(std::vector<Leaf*>& updateList) : mUpdateList(updateList) {}
		void Visit(Leaf* leaf) override;

	private:
		std::vector<Leaf*>& mUpdateList;
	};

	class DrawVisitor : public Tree::Visitor
	{
	public:
		DrawVisitor(RTreeScene& scene) : mScene(scene) {}
		void Visit(LeafData& data) override {mScene.Draw(data);}

	private:
		RTreeScene& mScene;
	};

	Tree mTree;
};

}
}

#endif // RTREESCENE_H
