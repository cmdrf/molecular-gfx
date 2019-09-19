/*	CpuParticleSystemImpl.h
	Copyright 2016-2017 Fabian Herb

	This file is part of Molecular Engine.
*/

#include "CpuParticleSystem.h"
#include "util/Matrix4.h"

namespace Gfx
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
void CpuParticleSystem<TRenderManager>::Execute()
{
	bool shadowPass = !mScoping["fragmentColor"_H];

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
		Binding<Attribute> position("vertexPositionAttr"_H, this);
		Binding<Attribute> particleAge("particleAge"_H, this);
		Binding<Uniform<float>> particleMaxAge("particleMaxAge"_H, this);
		Binding<Uniform<Vector3>> color("diffuseColor"_H, this);
		Binding<Uniform<Matrix4>> modelMatrix("modelMatrix"_H, this);
		Binding<Uniform<float>> pointWorldSize("pointWorldSize"_H, this);
		Binding<Output> glPointSizeOutput("gl_PointSize"_H, this);

		*position = Attribute(mVertexBuffer, mPositionInfo);
		*particleAge = Attribute(mVertexBuffer, mAgeInfo);
		**particleMaxAge = mMaxParticleAge;
		**color = mColor;
		**modelMatrix = Matrix4::Identity();
		**pointWorldSize = 0.5;

		PrepareProgram();
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
