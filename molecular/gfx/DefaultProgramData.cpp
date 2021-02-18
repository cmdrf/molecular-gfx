/*	DefaultProgramData.cpp

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

#include "DefaultProgramData.h"

namespace molecular
{
namespace gfx
{
using namespace programgenerator;
using namespace util;

void DefaultProgramData::FeedToGenerator(ProgramGenerator& generator)
{
	ProgramGenerator::Function specularArray;
	specularArray.stage = ProgramGenerator::Function::Stage::kFragmentStage;
	specularArray.source.push_back(
			"specularLighting = vec3(0.0, 0.0, 0.0);\n"
			"\tfor(int i = 0; i < lightColorArray.length(); i++)\n"
			"\t{\n"
			"\t\tvec3 reflected = reflect(lightDirectionArray[i], normalize(normal));\n"
			"\t\tspecularLighting += pow(max(0.0, dot(reflected, eyeDirection)), shininess) * lightColorArray[i];\n"
			"\t}");
	specularArray.inputs.push_back("lightDirectionArray"_H);
	specularArray.inputs.push_back("normal"_H);
	specularArray.inputs.push_back("eyeDirection"_H);
	specularArray.inputs.push_back("shininess"_H);
	specularArray.inputs.push_back("lightColorArray"_H);
	specularArray.output = "specularLighting"_H;
	generator.AddFunction(specularArray);

	ProgramGenerator::Function clipPlane;
	clipPlane.stage = ProgramGenerator::Function::Stage::kVertexStage;
	clipPlane.source.push_back("gl_ClipDistance[0] = dot(modelMatrix * vertexPosition, clipPlane0);");
	clipPlane.inputs.push_back("modelMatrix"_H);
	clipPlane.inputs.push_back("vertexPosition"_H);
	clipPlane.inputs.push_back("clipPlane0"_H);
	clipPlane.output = "gl_ClipDistance"_H;
	generator.AddFunction(clipPlane);

	ProgramGenerator::Function lightOnly;
	lightOnly.stage = ProgramGenerator::Function::Stage::kFragmentStage;
	lightOnly.source.push_back("fragmentColor = vec4(diffuseLighting, 1.0);");
	lightOnly.inputs.push_back("diffuseLighting"_H);
	lightOnly.output = "fragmentColor"_H;
	lightOnly.priority = -1;
	generator.AddFunction(lightOnly);

	ProgramGenerator::Function lightDirectionArray;
	lightDirectionArray.stage = ProgramGenerator::Function::Stage::kVertexStage;
	lightDirectionArray.source.push_back(
			"for(int i = 0; i < lightDirectionArray.length(); i++)\n"
			"\t{\n"
			"\t\tlightDirectionArray[i] = -normalize(lightPositionArray[i].xyz - (modelMatrix * vertexPosition).xyz);\n"
			"\t}");
	lightDirectionArray.inputs.push_back("lightPositionArray"_H);
	lightDirectionArray.inputs.push_back("vertexPosition"_H);
	lightDirectionArray.inputs.push_back("modelViewMatrix"_H);
	lightDirectionArray.output = "lightDirectionArray"_H;
	lightDirectionArray.outputArraySizeSource = "lightPositionArray"_H;
	generator.AddFunction(lightDirectionArray);

	generator.AddVariable("modelMatrix", "mat4");
	generator.AddVariable("modelViewMatrix", "mat4");
	generator.AddVariable("clipPlane0", "vec4");
	generator.AddVariable("shadowViewProjMatrix0", "mat4");

	generator.AddVariable("gl_ClipDistance", "float", true);

	generator.AddVariable("lightPosition0", "vec4");
	generator.AddVariable("lightDirection0", "vec3");
	generator.AddVariable("lightPositionArray", "vec4", true);
	generator.AddVariable("lightColorArray", "vec3", true);
	generator.AddVariable("lightAmbientArray", "vec3", true);
	generator.AddVariable("lightDirectionArray", "vec3", true);

	generator.AddVariable("vertexPosition", "vec4", false);

	generator.AddVariable("shininess", "float");
	generator.AddVariable("eyeDirection", "vec3");

	generator.AddVariable("diffuseLighting", "vec3");
	generator.AddVariable("specularLighting", "vec3");
	generator.AddVariable("fragmentColor", "vec4", false, ProgramGenerator::VariableInfo::Usage::kOutput);
}

}
}
