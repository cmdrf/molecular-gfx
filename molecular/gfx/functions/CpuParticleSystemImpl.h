/*	CpuParticleSystemImpl.h

MIT License

Copyright (c) 2018-2019 Fabian Herb

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

#ifndef MOLECULAR_CPUPARTICLESYSTEMIMPL_H
#define MOLECULAR_CPUPARTICLESYSTEMIMPL_H

#include "CpuParticleSystem.h"
#include <molecular/util/Matrix4.h>

namespace molecular
{
namespace gfx
{

template<class TRenderManager>
CpuParticleSystem<TRenderManager>::CpuParticleSystem(TRenderManager& manager) :
	DrawingFunction(manager),
	mVertexBuffer(mRenderer.CreateVertexBuffer()),
	mPositionInfo(VertexAttributeInfo::kFloat, 3, 0, sizeof(ParticleVec), 0),
	mAgeInfo(VertexAttributeInfo::kFloat, 1, 24, sizeof(ParticleVec), 0),
	mTaskQueue(manager.GetTaskQueue())
{
	mAdditive.setZero();
	mTransitionRandomSigma << 0, 0, 0, 0.1f, 0.1f, 0.1f, 0, 0;
	mEmissionRandomSigma << 1, 1, 1, 2, 2, 2, 0, 0;
}

template<class TRenderManager>
template<typename Iterator>
void CpuParticleSystem<TRenderManager>::SetEmitters(Iterator begin, Iterator end)
{
	mEmitters.clear();
	ParticleVec emitter;
	for(Iterator it = begin; it != end; ++it)
	{
		emitter << (*it)[0], (*it)[1], (*it)[2], 0, 0, 0, 0, 0;
		mEmitters.push_back(emitter);
	}
}

template<class TRenderManager>
CpuParticleSystem<TRenderManager>::~CpuParticleSystem()
{
	mTaskQueue.WaitUntilFinished(mSimulationFinishFlag);
	mRenderer.DestroyVertexBuffer(mVertexBuffer);
}

template<class TRenderManager>
void CpuParticleSystem<TRenderManager>::HandleExecute(Scope& scope)
{
	bool shadowPass = !scope.Has("fragmentColor"_H);

	if((shadowPass && mCastShadows) || (!shadowPass && !mCastShadows))
	{
		if(mTaskQueue.IsFinished(mSimulationFinishFlag))
		{
			// TODO: Keep buffer size constant
			if(!mParticles.empty())
				mVertexBuffer->Store(mParticles.data(), mParticles.size() * sizeof(ParticleVec), true);
			mNumParticles = mParticles.size();
			mSimulationTaskEmitters = mEmitters;
			auto simulate = [this]()
			{
				RemoveOld(mMaxParticleAge);
				const float t = 1.0f / 60.0f; // TODO
				Transition(t, mTransitionRandomSigma, mAdditive);
				Emission(t, mEmissionRandomSigma, mEmitsPerSecond);
			};
			mTaskQueue.EnqueueTask(simulate, mSimulationFinishFlag);
		}
	}

	if(!shadowPass || mCastShadows)
	{
		scope.Set("vertexPositionAttr"_H, Attribute(mVertexBuffer, mPositionInfo));
		scope.Set("particleAge"_H, Attribute(mVertexBuffer, mAgeInfo));
		scope.Set("particleMaxAge"_H, Uniform<float>(mMaxParticleAge));
		scope.Set("diffuseColor"_H, Uniform<Vector3>(mColor));
		scope.Set("modelMatrix"_H, Uniform<Matrix4>(Matrix4::Identity()));
		scope.Set("pointWorldSize"_H, Uniform<float>(0.5));
		scope.Set("gl_PointSize"_H, Output());

		PrepareProgram(scope);
		mRenderer.SetBlending(true, RenderCmdSink::kOne, RenderCmdSink::kOne);
		mRenderer.SetDepthState(true, false);
		mRenderer.Draw(IndexBufferInfo::Mode::kPoints, mNumParticles);
		mRenderer.SetBlending(false);
		mRenderer.SetDepthState(true, true);
	}
}

template<class TRenderManager>
void CpuParticleSystem<TRenderManager>::SetWind(Vector3 wind)
{
	mAdditive(0) = wind[0];
	mAdditive(1) = wind[1];
	mAdditive(2) = wind[2];
}

template<class TRenderManager>
void CpuParticleSystem<TRenderManager>::SetAcceleration(Vector3 acceleration)
{
	mAdditive(3) = acceleration[0];
	mAdditive(4) = acceleration[1];
	mAdditive(5) = acceleration[2];
}

template<class TRenderManager>
void CpuParticleSystem<TRenderManager>::RemoveOld(float maxParticleAge)
{
	for(int i = 0; i < mParticles.size(); ++i)
	{
		if(mParticles[i](kIndexAge) > mMaxParticleAge)
		{
			if(mParticles.size() > 1)
				mParticles[i] = mParticles.back();
			mParticles.pop_back();
		}
	}
}

template<class TRenderManager>
void CpuParticleSystem<TRenderManager>::Transition(float t, ParticleVec randomSigma, ParticleVec additive)
{
	for(auto& particle: mParticles)
	{
		particle.head(3) += particle.segment(kIndexVelX, 3) * t;
		particle += additive * t;
		particle(kIndexAge) += t;
		particle += ParticleVec::Random().cwiseProduct(randomSigma);
	}
}

template<class TRenderManager>
void CpuParticleSystem<TRenderManager>::Emission(float t, ParticleVec randomSigma, float emitsPerSecond)
{
	int emissionsPerEmitter = emitsPerSecond * t;
	for(auto& emitter: mSimulationTaskEmitters)
	{
		for(int i = 0; i < emissionsPerEmitter; ++i)
		{
			if(mParticles.size() >= kMaxParticles)
				return;

			mParticles.push_back(emitter + ParticleVec::Random().cwiseProduct(randomSigma));
		}
	}
}

}
}

#endif
