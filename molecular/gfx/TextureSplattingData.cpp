/*	TextureSplattingData.cpp

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

#include "TextureSplattingData.h"
#include "DefaultProgramData.h"

namespace molecular
{
namespace gfx
{
using namespace programgenerator;

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
}
