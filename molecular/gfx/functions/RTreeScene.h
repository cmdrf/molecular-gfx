/*	RTreeScene.h
	Copyright 2012-2014 Fabian Herb

	This file is part of Molecular Engine.
*/

#ifndef RTREESCENE_H
#define RTREESCENE_H

#include <molecular/gfx/RenderFunction.h>
#include <molecular/util/RTree.h>
#include <molecular/util/Frustum.h>

namespace molecular
{
class Matrix4;

namespace Gfx
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

	typedef RTree<LeafData> Tree;

public:
	typedef Tree::Leaf Leaf;

	RTreeScene(RenderManager& manager);

	void Execute() override;
	AxisAlignedBox GetBounds() const override;
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
		Tree::AcceptorOutput operator() (const AxisAlignedBox& box)
		{
			return Tree::kAccept;
		}
	};

	class FrustumAcceptor
	{
	public:
		FrustumAcceptor(const Frustum& frustum) : mFrustum(frustum) {}
		Tree::AcceptorOutput operator() (const AxisAlignedBox& box) const
		{
			Plane::IntersectStatus result = mFrustum.Check(box);
			if(result == Plane::kInside)
				return Tree::kAccept;
			else if(result == Plane::kOutside)
				return Tree::kDiscard;
			else
				return Tree::kDescend;
		}

	private:
		const Frustum& mFrustum;
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
