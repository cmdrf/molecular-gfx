/*	MtlFile.h

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

#ifndef MOLECULAR_MTLFILE_H
#define MOLECULAR_MTLFILE_H

#include <string>
#include <vector>

namespace molecular
{
namespace util
{

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

}
}

#endif // MOLECULAR_MTLFILE_H
