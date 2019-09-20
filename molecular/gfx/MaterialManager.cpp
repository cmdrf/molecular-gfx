/*	MaterialManager.cpp
	Copyright 2014-2016 Fabian Herb

	This file is part of Molecular Engine.
*/

#include "MaterialManager.h"
#include "Material.h"
#include <sstream>
#include <molecular/util/MemoryStreamStorage.h>
#include <molecular/util/Logging.h>

namespace molecular
{
namespace Gfx
{

MaterialManager::~MaterialManager()
{
}

void MaterialManager::ReadMtlFile(const Blob& blob)
{
	const char* data = static_cast<const char*>(blob.GetData());
	MtlFile mtlFile(data, data + blob.GetSize());
	ReadFile(mtlFile);
}

void MaterialManager::ReadIniFile(const Blob& blob)
{
	MemoryReadStorage storage(blob.GetData(), blob.GetSize());
	IniFile file(storage);
	ReadFile(file);
}

void MaterialManager::ReadFile(IniFile& file)
{
	const IniFile::SectionMap& sections = file.GetAllSections();
	IniFile::SectionMap::const_iterator section = sections.begin();
	while(section != sections.end())
	{
		Material* material = new Material(mTextureManager, mScoping);
		mMaterials[HashUtils::MakeHash(section->first)].reset(material);
		IniFile::ValueMap::const_iterator it = section->second.begin();
		while(it != section->second.end())
		{
			AddVariable(material, HashUtils::MakeHash(it->first), it->second);
			++it;
		}
		++section;
	}
}

void MaterialManager::ReadFile(MtlFile& file)
{
	for(const MtlFile::Material& mat: file)
	{
		Material* material = new Material(mTextureManager, mScoping);
		if(mat.hasDiffuse)
			material->SetUniform("diffuseColor"_H, Vector4(Vector3(mat.diffuse)));
		if(mat.hasAmbient)
			material->SetUniform("ambientColor"_H, Vector3(mat.ambient));
		if(mat.hasSpecular)
			material->SetUniform("specularColor"_H, Vector3(mat.specular));
		mMaterials[HashUtils::MakeHash(mat.name)].reset(material);
	}
}

Material* MaterialManager::GetMaterial(Hash hash) const
{
	auto it = mMaterials.find(hash);
	if(it == mMaterials.end())
		return nullptr;
	else
		return it->second.get();
}

Material* MaterialManager::GetMaterial(const std::string& material) const
{
	Material* mat = GetMaterial(HashUtils::MakeHash(material));
	if(!mat)
		LOG(ERROR) << "Material \"" << material << "\" not found";
	return mat;
}

void MaterialManager::AddVariable(Material* material, Hash key, const std::string& value)
{
	assert(material);

	size_t paraPos = value.find_first_of('(');
	if(paraPos == std::string::npos)
	{
		LOG(ERROR) << "Unrecognized \"" << value << "\"";
		return;
	}
	std::string function = value.substr(0, paraPos);
	size_t paraClosePos = value.find_first_of(')', paraPos + 1);
	if(paraClosePos == std::string::npos)
	{
		LOG(ERROR) << "Unrecognized \"" << value << "\"";
		return;
	}

	std::string parameters = value.substr(paraPos + 1, paraClosePos - paraPos - 1);
	std::istringstream parametersIss(parameters);
	char comma;
	if(function == "float")
	{
		float value = 0.0;
		parametersIss >> value;
		material->SetUniform(key, value);
	}
	else if(function == "vec2")
	{
		Vector2 vec;
		parametersIss >> vec[0] >> comma >> vec[1];
		material->SetUniform(key, vec);
	}
	else if(function == "vec3")
	{
		Vector3 vec;
		parametersIss >> vec[0] >> comma >> vec[1] >> comma >> vec[2];
		material->SetUniform(key, vec);
	}
	else if(function == "vec4")
	{
		Vector4 vec;
		parametersIss >> vec[0] >> comma >> vec[1] >> comma >> vec[2] >> comma >> vec[3];
		material->SetUniform(key, vec);
	}
	else if(function == "texture")
	{
		material->SetTexture(key, HashUtils::MakeHash(parameters));
	}
	else if(function == "int")
	{
		int value = 0;
		parametersIss >> value;
		material->SetUniform(key, value);
	}
	else if(function == "ivec2")
	{
		IntVector2 ivec;
		parametersIss >> ivec[0] >> comma >> ivec[1];
		material->SetUniform(key, ivec);
	}
	else if(function == "ivec3")
	{
		IntVector3 ivec;
		parametersIss >> ivec[0] >> comma >> ivec[1] >> comma >> ivec[2];
		material->SetUniform(key, ivec);
	}
	else if(function == "ivec4")
	{
		IntVector4 ivec;
		parametersIss >> ivec[0] >> comma >> ivec[1] >> comma >> ivec[2] >> comma >> ivec[3];
		material->SetUniform(key, ivec);
	}
	else if(function == "enum")
	{
		Hash hash = HashUtils::MakeHash(parameters);
		material->SetUniform(key, hash);
	}
}

}
}
