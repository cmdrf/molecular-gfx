/*	SetupLight.cpp

MIT License

Copyright (c) 2019-2020 Fabian Herb

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

#include "SetupLight.h"

namespace molecular
{
namespace gfx
{

void SetupLight::Execute()
{
	Binding<Uniform<Hash>> blendMode("blendMode"_H, this);
	**blendMode = "none"_H;

	if(mDirectionalLightEnabled)
	{
		Binding<Uniform<Vector3> > lightDirection0("lightDirection0"_H, this);
		Binding<Uniform<Vector3> > lightColor0("lightColor0"_H, this);

		**lightDirection0 = mLightDirection;
		**lightColor0 = mLightColor;

		mCallee->Execute();
	}
	else
		mCallee->Execute();
}

void SetupLight::SetDirectionalLight(bool enable, const Vector3& direction, const Vector3& color)
{
	mDirectionalLightEnabled = enable;
	mLightDirection = direction.Normalized();
	mLightColor = color;
}

void SetupLight::Set(Hash key, bool value)
{
	if(key == "directionalLightEnabled"_H)
		mDirectionalLightEnabled = value;
	else
		throw std::runtime_error("Unknown bool variable in SetupLight");
}

}
}
