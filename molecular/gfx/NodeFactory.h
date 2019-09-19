/*	NodeFactory.h
	Copyright 2015-2017 Fabian Herb

	This file is part of Molecular Engine.
*/

#ifndef NODEFACTORY_H
#define NODEFACTORY_H

#include "RenderFunction.h"
namespace Gfx
{

template<class TRenderManager>
class NodeFactory
{
public:
	NodeFactory(TRenderManager& renderManager) : mRenderManager(renderManager) {}

	template<class T>
	void Register(const char* name)
	{
		mInstancers[HashUtils::MakeHash(name)].reset(new Instancer<T>);
	}

	RenderFunction* Instantiate(Hash name)
	{
		auto it = mInstancers.find(name);
		if(it == mInstancers.end())
			return nullptr;
		else
			return it->second->Instantiate(mRenderManager);
	}

private:
	class InstancerBase
	{
	public:
		virtual RenderFunction* Instantiate(TRenderManager& renderManager) = 0;
	};

	template<class T>
	class Instancer : public InstancerBase
	{
	public:
		RenderFunction* Instantiate(TRenderManager& renderManager) override
		{
			return new T(renderManager);
		}
	};

	std::unordered_map<Hash, std::unique_ptr<InstancerBase>> mInstancers;
	TRenderManager& mRenderManager;
};

}

#endif // NODEFACTORY_H
