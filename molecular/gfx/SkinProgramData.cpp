/*	SkinProgramData.cpp

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

#include "SkinProgramData.h"
#include "DefaultProgramData.h"

namespace molecular
{
namespace gfx
{
using namespace util;
using namespace programgenerator;

void SkinProgramData::FeedToGenerator(ProgramGenerator& generator)
{
	generator.AddVariable("beckmannTexture", "sampler2D");
	generator.AddVariable("specularBrightness", "float");
	generator.AddVariable("specularBrightnessTexture", "sampler2D");

	ProgramGenerator::Function specularBrightness;
	specularBrightness.stage = ProgramGenerator::Function::Stage::kFragmentStage;
	specularBrightness.source =
			"specularBrightness = texture(specularBrightnessTexture, vertexUv0).r;";
	specularBrightness.inputs.push_back("specularBrightnessTexture"_H);
	specularBrightness.inputs.push_back("vertexUv0"_H);
	specularBrightness.output = "specularBrightness"_H;
	generator.AddFunction(specularBrightness);

	ProgramGenerator::Function specular;
	specular.stage = ProgramGenerator::Function::Stage::kFragmentStage;
	specular.source =
		"float specularResult = 0.0;\n"
		"\tfloat ndotl = dot( normalize(normal), -normalize(lightDirection0) );\n"
		"\tif( ndotl > 0.0 )\n"
		"\t{\n"
		"\t\tvec3 h = -normalize(lightDirection0) + normalize(eyeDirection);\n" // Unnormalized half-way vector
		"\t\tvec3 H = normalize( h );\n"
		"\t\tfloat ndoth = dot( normalize(normal), H );\n"
		"\t\tfloat PH = pow( 2.0 * texture(beckmannTexture, vec2(ndoth, 0.145)).r, 10.0 );\n"
		"\t\tfloat base = 1.0 - dot( eyeDirection, H );\n"
		"\t\tfloat exponential = pow( base, 5.0 );\n"
		"\t\tfloat F = exponential + 0.028 * ( 1.0 - exponential );\n"
		"\t\tfloat frSpec = max( PH * F / dot( h, h ), 0 );\n"
		"\t\tspecularResult = ndotl * frSpec * 0.75;\n" // BRDF * dot(N,L) * rho_s
		"\t}\n"
		"\tspecularLighting = specularResult * incomingLight0;";
	specular.inputs.push_back("lightDirection0"_H);
	specular.inputs.push_back("normal"_H);
	specular.inputs.push_back("eyeDirection"_H);
	specular.inputs.push_back("incomingLight0"_H);
//	specular.inputs.push_back(kSpecularBrightness);
	specular.inputs.push_back("beckmannTexture"_H);
	specular.output = "specularLighting"_H;
	generator.AddFunction(specular);
}

}
}
