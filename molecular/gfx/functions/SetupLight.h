/*	SetupLight.h

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

#ifndef MOLECULAR_GFX_SETUPLIGHT_H
#define MOLECULAR_GFX_SETUPLIGHT_H

#include <molecular/gfx/RenderFunction.h>
#include <molecular/util/Quaternion.h>
#include <molecular/util/Matrix4.h>

namespace molecular
{
namespace gfx
{

/// Sets up a single, directional light
/** Sets uniforms lightDirection0 and lightColor0. */
class SetupLight : public SingleCalleeRenderFunction
{
public:
	template<class TRenderManager>
	SetupLight(TRenderManager& manager) :
		SingleCalleeRenderFunction(manager)
	{
	}

	void Execute() override;

	/// Set light parameters
	void SetDirectionalLight(bool enable, const Vector3& direction = Vector3(1,-1,-1), const Vector3 &color = Vector3(1,1,1));

	void Set(Hash key, bool value) override;

private:
	Vector3 mLightDirection = Vector3(1,-1,-1);
	Vector3 mLightColor = Vector3(1,1,1);
	bool mDirectionalLightEnabled = true;
};

}
}

#endif // MOLECULAR_GFX_SETUPLIGHT_H
