/*	SetUniforms.h

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

#ifndef MOLECULAR_SETUNIFORMS_H
#define MOLECULAR_SETUNIFORMS_H

#include <molecular/gfx/RenderFunction.h>
#include <molecular/gfx/Material.h>

namespace molecular
{
namespace gfx
{

/// Sets arbitrary uniform variables before calling the callee
/** Variables are stored as a Material. */
class SetUniforms : public SingleCalleeRenderFunction
{
public:
	template<class TRenderManager>
	SetUniforms(TRenderManager& manager) :
		SingleCalleeRenderFunction(manager),
		mUniforms(manager)
	{
	}

	void Execute() override;

	template<class T>
	void SetUniform(Hash hash, const T& variable)
	{
		mUniforms.SetUniform(hash, variable);
	}

private:
	Material mUniforms;
};

}
}

#endif // MOLECULAR_SETUNIFORMS_H
