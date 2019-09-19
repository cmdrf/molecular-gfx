/*	GfxContext.h
	Copyright 2015-2016 Fabian Herb

	This file is part of Molecular Engine.
*/

#ifndef GFXCONTEXT_H
#define GFXCONTEXT_H

#include "NodeFactory.h"
#include "functions/SceneSetup.h"
#include "functions/ShadowMapping.h"
#include "functions/ViewSetup.h"
#include "GfxFile.h"

namespace Gfx
{

/// Loads GFX setup text files
template<class TRenderManager>
class GfxContext
{
public:
	GfxContext(TRenderManager& renderManager);

	/// Adds a RenderFunction that can referenced in the file
	void AddExternalReference(Hash name, RenderFunction* function);

	void LoadFile(const char* data, std::size_t size);

	RenderFunction* GetNode(Hash name);

private:
	void AddChild(int parent, RenderFunction* child, GfxFile& file);

	NodeFactory<TRenderManager> mNodeFactory;
	std::vector<std::unique_ptr<RenderFunction>> mNodes;
	std::unordered_map<Hash, RenderFunction*> mReferences;
};

/*****************************************************************************/

template<class TRenderManager>
GfxContext<TRenderManager>::GfxContext(TRenderManager& renderManager) :
	mNodeFactory(renderManager)
{
	mNodeFactory.template Register<SceneSetup>("SceneSetup");
	mNodeFactory.template Register<ShadowMapping>("ShadowMapping");
	mNodeFactory.template Register<ViewSetup>("ViewSetup");
}

template<class TRenderManager>
void GfxContext<TRenderManager>::AddExternalReference(Hash name, RenderFunction* function)
{
	mReferences[name] = function;
}

template<class TRenderManager>
void GfxContext<TRenderManager>::LoadFile(const char* data, std::size_t size)
{
	GfxFile file(data, size);
	for(auto& node: file.GetNodes())
	{
		Hash hash = HashUtils::MakeHash(node.classBegin, node.classEnd);
		RenderFunction* f = mNodeFactory.Instantiate(hash);
		if(!f)
			throw std::runtime_error("Unknown node class " + std::string(node.classBegin, node.classEnd));
		mNodes.emplace_back(f);
		if(node.parent >= 0)
			AddChild(node.parent, f, file);

		if(node.nameBegin && node.nameEnd)
		{
			Hash nameHash = HashUtils::MakeHash(node.nameBegin, node.nameEnd);
			if(mReferences.find(nameHash) != mReferences.end())
				throw std::runtime_error("Node name \"" + std::string(node.nameBegin, node.nameEnd) + "\" already taken.");
			mReferences[nameHash] = f;
		}
	}

	for(auto& variable: file.GetVariables())
	{
		RenderFunction& node = *mNodes.at(variable.parent);
		Hash hash = HashUtils::MakeHash(variable.nameBegin, variable.nameEnd);
		switch(variable.type)
		{
		case GfxFile::Variable::kBool:
			node.Set(hash, variable.value.boolean);
			break;
		case GfxFile::Variable::kInteger:
		case GfxFile::Variable::kReal:
		case GfxFile::Variable::kVec2:
		case GfxFile::Variable::kVec3:
		case GfxFile::Variable::kVec4:
		case GfxFile::Variable::kIVec2:
		case GfxFile::Variable::kIVec3:
		case GfxFile::Variable::kIVec4:
		default:
			throw std::runtime_error("Cannot set variable " + std::string(variable.nameBegin, variable.nameEnd));
		}

		// TODO
	}

	for(auto& reference: file.GetReferences())
	{
		Hash hash = HashUtils::MakeHash(reference.nameBegin, reference.nameEnd);
		auto it = mReferences.find(hash);
		if(it == mReferences.end())
			throw std::runtime_error("Unknown node \"" + std::string(reference.nameBegin, reference.nameEnd) + "\"");

		AddChild(reference.parent, it->second, file);
	}
}

template<class TRenderManager>
RenderFunction* GfxContext<TRenderManager>::GetNode(Hash name)
{
	auto it = mReferences.find(name);
	if(it == mReferences.end())
		return nullptr;
	else
		return it->second;
}

template<class TRenderManager>
void GfxContext<TRenderManager>::AddChild(int parent, RenderFunction* child, GfxFile& file)
{
	assert(parent < mNodes.size());
	if(SingleCalleeRenderFunction* p = dynamic_cast<SingleCalleeRenderFunction*>(mNodes[parent].get()))
		p->SetCallee(child);
	else
	{
		auto parentNode = file.GetNodes()[parent];
		std::string parentClass(parentNode.classBegin, parentNode.classEnd);
		throw std::runtime_error("Node class " + parentClass + " cannot have children.");
	}
}

}

#endif // GFXCONTEXT_H

