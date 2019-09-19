/*	RTreeScene.cpp
	Copyright 2012-2014 Fabian Herb

	This file is part of Molecular Engine.
*/

#include "RTreeScene.h"
#include <molecular/gfx/DefaultProgramData.h>
#include <molecular/util/Matrix4.h>

namespace molecular
{
RTreeScene::RTreeScene(RenderManager& manager) :
	RenderFunction(manager)
{
}

void RTreeScene::Execute()
{
	// Update bounds:
	std::vector<Leaf*> leafsToUpdate;
	UpdateVisitor visitor(leafsToUpdate);
	AllAcceptor acceptor;
	mTree.Query(acceptor, visitor);
	for(std::vector<Leaf*>::iterator it = leafsToUpdate.begin(); it != leafsToUpdate.end(); ++it)
	{
		Leaf* leaf = *it;
		LeafData& data = leaf->GetData();
		mTree.Update(leaf, data.function->GetBounds());
		data.boundsUpdated = mRenderManager.GetFramecounter();
	}

	// Draw:
	const Uniform<Matrix4>* projMatrix = GetVariable<Uniform<Matrix4> >(DefaultProgramData::kProjectionMatrix);
	const Uniform<Matrix4>* viewMatrix = GetVariable<Uniform<Matrix4> >(DefaultProgramData::kViewMatrix);

	if(!projMatrix || !viewMatrix)
		return;

	Matrix4 viewProjMatrix = **projMatrix * **viewMatrix;
	Draw(viewProjMatrix);
}

AxisAlignedBox RTreeScene::GetBounds() const
{
	return AxisAlignedBox();
}

bool RTreeScene::BoundsChangedSince(int framecounter) const
{
	return false;
}


RTreeScene::Leaf* RTreeScene::Insert(RenderFunction* function)
{
	LeafData data;
	data.boundsUpdated = mRenderManager.GetFramecounter();
	data.function = function;
	return mTree.Insert(data, function->GetBounds());
}

void RTreeScene::Remove(Leaf* leaf)
{
	mTree.Remove(leaf);
}

void RTreeScene::Draw(LeafData& data)
{
	data.function->Execute();
}

void RTreeScene::Draw(const Matrix4& viewProjectionMatrix)
{
	Frustum frustum(viewProjectionMatrix);
//	FrustumAcceptor acceptor(frustum);
	AllAcceptor acceptor;
	DrawVisitor visitor(*this);
	mTree.Query(acceptor, visitor);
}

void RTreeScene::UpdateVisitor::Visit(Leaf* leaf)
{
	assert(leaf);
	LeafData& data = leaf->GetData();
	if(data.function->BoundsChangedSince(data.boundsUpdated))
		mUpdateList.push_back(leaf);
}

}
