/*	TextureSplattingData.cpp
	Copyright 2012-2018 Fabian Herb

	This file is part of Molecular Engine.
*/

#include "TextureSplattingData.h"
#include "DefaultProgramData.h"

namespace molecular
{

void TextureSplattingData::FeedToGenerator(ProgramGenerator& generator)
{
	generator.AddVariable("splattingMapRgb0", "sampler2D");
	generator.AddVariable("splattingMapRgb1", "sampler2D");
//	generator.AddVariable("splattingMapRgb2", "sampler2D");
	generator.AddVariable("splattingMapRgba0", "sampler2D");
	generator.AddVariable("splattingMapRgba1", "sampler2D");
//	generator.AddVariable("splattingMapRgba2", "sampler2D");
	generator.AddVariable("splattingTexturesArray", "sampler2D", true);
	generator.AddVariable("splattingScale", "float");

	ProgramGenerator::Function rgb2;
	rgb2.stage = ProgramGenerator::Function::Stage::kFragmentStage;
	rgb2.inputs.push_back("splattingMapRgb0"_H);
	rgb2.inputs.push_back("splattingMapRgb1"_H);
	rgb2.inputs.push_back("splattingTexturesArray"_H);
	rgb2.inputs.push_back("splattingScale"_H);
	rgb2.inputs.push_back("vertexUv0"_H);
	rgb2.output = "diffuseColor"_H;
	rgb2.source =
			"vec3 cov0 = texture(splattingMapRgb0, vertexUv0).rgb;\n"
			"\tvec3 cov1 = texture(splattingMapRgb1, vertexUv0).rgb;\n"
			"\tvec2 scaledCoord = vertexUv0 * splattingScale;\n"
//			"\tdiffuseColor = texture(splattingTexturesArray[5], scaledCoord).rgb;"
			"\tdiffuseColor = (texture(splattingTexturesArray[0], scaledCoord).rgb * cov0.x\n"
			"\t\t+ texture(splattingTexturesArray[1], scaledCoord).rgb * cov0.y\n"
			"\t\t+ texture(splattingTexturesArray[2], scaledCoord).rgb * cov0.z\n"
			"\t\t+ texture(splattingTexturesArray[3], scaledCoord).rgb * cov1.x\n"
			"\t\t+ texture(splattingTexturesArray[4], scaledCoord).rgb * cov1.y\n"
			"\t\t+ texture(splattingTexturesArray[5], scaledCoord).rgb * cov1.z);";
	generator.AddFunction(rgb2);
}
}
