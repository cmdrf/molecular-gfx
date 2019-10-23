/*	MtlFile.cpp

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

#include "MtlFile.h"
#include <molecular/util/Parser.h>
#include <iostream>
#include <stdexcept>
#include <cstdlib>

namespace molecular
{

using namespace Parser;

template<char a, char b>
using Keyword2 = Concatenation<Char<a>, Char<b>>;

template<char a, char b, char c, char d, char e>
using Keyword5 = Concatenation<Char<a>, Char<b>, Char<c>, Char<d>, Char<e>>;

template<char a, char b, char c, char d, char e, char f>
using Keyword6 = Concatenation<Char<a>, Char<b>, Char<c>, Char<d>, Char<e>, Char<f>>;

using NonWhitespace = Alternation<Alpha, Digit, Char<'_'>, Char<'-'>, Char<'.'>>;
using String = Concatenation<NonWhitespace, Repetition<NonWhitespace>>;

class NonNewline
{
public:
	template<class Iterator>
	static bool Parse(Iterator& begin, Iterator end, void*)
	{
		if(begin == end)
			return false;

		if(*begin != '\n')
		{
			begin++;
			return true;
		}
		return false;
	}
};

MtlFile::MtlFile(const char* begin, const char* end)
{
	using Newmtl = Keyword6<'n', 'e', 'w', 'm', 't', 'l'>;
	using Ns = Keyword2<'N', 's'>;
	using Ka = Keyword2<'K', 'a'>;
	using Kd = Keyword2<'K', 'd'>;
	using Ks = Keyword2<'K', 's'>;
	using Illum = Keyword5<'i', 'l', 'l', 'u', 'm'>;
//	using MapKa = Keyword6<'m', 'a', 'p', '_', 'K', 'a'>;
//	using MapKd = Keyword6<'m', 'a', 'p', '_', 'K', 'd'>;
//	using MapKs = Keyword6<'m', 'a', 'p', '_', 'K', 's'>;
//	using MapNs = Keyword6<'m', 'a', 'p', '_', 'N', 's'>;
	using TrD = Alternation<Keyword2<'T', 'r'>, Char<'d'>>;

	using NewmtlDecl = Concatenation<Newmtl, Whitespace, Action<String, kNewmtlValue>>;
	using NsDecl = Concatenation<Ns, Whitespace, Action<Real, kNsValue>>;
	using KaDecl = Concatenation<Ka, Whitespace, Action<Real, kKaValueR>, Whitespace, Action<Real, kKaValueG>, Whitespace, Action<Real, kKaValueB>>;
	using KdDecl = Concatenation<Kd, Whitespace, Action<Real, kKdValueR>, Whitespace, Action<Real, kKdValueG>, Whitespace, Action<Real, kKdValueB>>;
	using KsDecl = Concatenation<Ks, Whitespace, Action<Real, kKsValueR>, Whitespace, Action<Real, kKsValueG>, Whitespace, Action<Real, kKsValueB>>;
	using IllumDecl = Concatenation<Illum, Whitespace, Action<Integer, kIllumValue>>;
	using TrDecl = Concatenation<TrD, Whitespace, Action<Real, kTrValue>>;

	using Declaration = Concatenation<Alternation<NewmtlDecl, NsDecl, KaDecl, KdDecl, KsDecl, IllumDecl, TrDecl>, Char<'\n'>>;

	using Comment = Concatenation<Char<'#'>, Repetition<NonNewline>, Char<'\n'>>;
	using Line = Concatenation<Option<Whitespace>, Alternation<Declaration, Comment>>;
	using File = Repetition<Line>;

	bool success = File::Parse(begin, end, this);

	if(!success)
		throw std::runtime_error("Error reading material file");
}

void MtlFile::ParserAction(int action, const char* begin, const char* end)
{
	if(action == kNewmtlValue)
	{
		mMaterials.push_back(Material(std::string(begin, end - begin)));
	}
	else
	{
		if(mMaterials.empty())
			throw std::runtime_error("Property declaration before newmtl");
		Material& mat = mMaterials.back();
		switch (action)
		{
		case kNsValue:
			mat.hasSpecularExponent = true;
			mat.specularExponent = strtof(begin, nullptr);
			break;

		case kKaValueR:
			mat.hasAmbient = true;
			mat.ambient[0] = strtof(begin, nullptr);
			break;

		case kKaValueG:
			mat.hasAmbient = true;
			mat.ambient[1] = strtof(begin, nullptr);
			break;

		case kKaValueB:
			mat.hasAmbient = true;
			mat.ambient[2] = strtof(begin, nullptr);
			break;

		case kKdValueR:
			mat.hasDiffuse = true;
			mat.diffuse[0] = strtof(begin, nullptr);
			break;

		case kKdValueG:
			mat.hasDiffuse = true;
			mat.diffuse[1] = strtof(begin, nullptr);
			break;

		case kKdValueB:
			mat.hasDiffuse = true;
			mat.diffuse[2] = strtof(begin, nullptr);
			break;

		case kKsValueR:
			mat.hasSpecular = true;
			mat.specular[0] = strtof(begin, nullptr);
			break;

		case kKsValueG:
			mat.hasSpecular = true;
			mat.specular[1] = strtof(begin, nullptr);
			break;

		case kKsValueB:
			mat.hasSpecular = true;
			mat.specular[2] = strtof(begin, nullptr);
			break;

		case kIllumValue:
			mat.hasIlluminationModel = true;
			mat.illuminationModel = strtol(begin, nullptr, 10);
			break;

		case kTrValue:
			mat.hasTransparency = true;
			mat.transparency = strtof(begin, nullptr);
			break;
		}
	}
}

}
