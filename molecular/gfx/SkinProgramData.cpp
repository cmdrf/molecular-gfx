/*	SkinProgramData.cpp
	Copyright 2013-2018 Fabian Herb

	This file is part of Molecular Engine.
*/

#include "SkinProgramData.h"
#include "DefaultProgramData.h"

using namespace molecular;

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
