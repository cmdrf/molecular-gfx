/*	MtlFile.h
	Copyright 2015 Fabian Herb

	This file is part of Molecular Engine.
*/

#ifndef MTLFILE_H
#define MTLFILE_H

#include <string>
#include <vector>

/// Wavefront material file.
/** @todo Read texture declarations */
class MtlFile
{
public:
	struct Texture
	{
		bool blendHorizontally = true;
		bool blendVertically = true;
		float boost;
		float baseValue = 0.0f;
		float gainValue = 1.0f;
		float origin[3] = {0.0f, 0.0f, 0.0f};
		float scale[3] = {1.0f, 1.0f, 1.0f};
		float turbulence[3] = {0.0f, 0.0f, 0.0f};
		int texres;
	};

	struct Material
	{
		Material(const std::string inName = "") :
			name(inName)
		{}

		std::string name;
		bool hasDiffuse = false;
		float diffuse[3] = {0,0,0};
		bool hasAmbient = false;
		float ambient[3] = {0,0,0};
		bool hasSpecular = false;
		float specular[3] = {0,0,0};
		bool hasSpecularExponent = false;
		float specularExponent = 0.0;
		bool hasTransparency = false;
		float transparency = 1.0;
		bool hasIlluminationModel = false;
		int illuminationModel = 2;
	};

	MtlFile(const char* begin, const char* end);

	const Material& GetMaterial(const std::string& name) const;

	std::vector<Material>::const_iterator begin() const {return mMaterials.begin();}
	std::vector<Material>::const_iterator end() const {return mMaterials.end();}

	void ParserAction(int action, const char* begin, const char* end);

private:
	/// Action IDs for parser actions
	/** @see ParserAction */
	enum
	{
		kNewmtlValue,
		kNsValue,
		kKaValueR,
		kKaValueG,
		kKaValueB,
		kKdValueR,
		kKdValueG,
		kKdValueB,
		kKsValueR,
		kKsValueG,
		kKsValueB,
		kIllumValue,
		kTrValue
	};

	std::vector<Material> mMaterials;
};

#endif // MTLFILE_H
