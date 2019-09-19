/*	ParticleSystem.cpp
	Copyright 2015-2017 Fabian Herb

	This file is part of Molecular Engine.
*/

#define NOMINMAX
#include "ParticleSystem.h"
#include <molecular/util/Matrix4.h>
#include <cstddef> // offsetof

namespace molecular
{
namespace Gfx
{

const VertexAttributeInfo ParticleSystem::mVertexAttributeInfos[] =
{
	{VertexAttributeInfo::kUInt8, 3, offsetof(ParticleSystem::Particle, color), sizeof(ParticleSystem::Particle), 0, false},
	{VertexAttributeInfo::kFloat, 1, offsetof(ParticleSystem::Particle, age), sizeof(ParticleSystem::Particle), 0, false},
	{VertexAttributeInfo::kFloat, 3, offsetof(ParticleSystem::Particle, position), sizeof(ParticleSystem::Particle), 0, false},
	{VertexAttributeInfo::kFloat, 3, offsetof(ParticleSystem::Particle, velocity), sizeof(ParticleSystem::Particle), 0, false}
};

ParticleSystem::~ParticleSystem()
{
	mRenderer.DestroyProgram(mBillboardProgram);
	mRenderer.DestroyProgram(mEmitterProgram);
	mRenderer.DestroyProgram(mFeedbackProgram);
	mRenderer.DestroyTexture(mRandomTexture);
	mRenderer.DestroyTexture(mParticleTexture);
	mRenderer.DestroyTexture(mFbmTexture);
}

void ParticleSystem::Execute()
{
	if(!mScoping["fragmentColor"_H])
		return; // Do not execute in shadow pass

	Update(1.0f / 60.0f);
	Draw();
}

void ParticleSystem::Draw()
{
	if (mEmittersCount != mParams.emittersCount)
		SeedEmitters(mParams.emittersCount);

	if (!mIsStopped)
		AdvanceFeedback();

	if (mCurrentBuffers > 1)
		DrawParticles();

	if (!mIsStopped)
		mCurrentBuffers++;
}



void ParticleSystem::InitProgram(RenderCmdSink::Program* program, const std::string& vertSrc, const std::string& geomSrc)
{
	RenderCmdSink::Program::ShaderSourceItem feedbackItems[2] =
	{
		{RenderCmdSink::Program::kVertexShader, vertSrc.data(), vertSrc.size()},
		{RenderCmdSink::Program::kGeometryShader, geomSrc.data(), geomSrc.size()}
	};

	const char* tfOutputs[4] =
	{
		"gs_TypeColor",
		"gs_Age",
		"gs_Position",
		"gs_Velocity"
	};
	program->Store(feedbackItems, 2, tfOutputs, 4);

}

void ParticleSystem::InitParticleBuffers()
{
	assert(this);
	std::vector<Particle> p(kMaxParticles); // Do not use array on stack!
	memset(p.data(), 0, kMaxParticles * sizeof(Particle));

	mEmitterFeedbackBuffer = mRenderer.CreateVertexBuffer();
	mEmitterFeedbackBuffer->Store(p.data(), kMaxParticles * sizeof(Particle));
	mEmitterFeedback = mRenderer.CreateTransformFeedback();

	for(int n = 0; n < 2; n++)
	{
		mParticleFeedbacks[n] = mRenderer.CreateTransformFeedback();
		mParticleBuffers[n] = mRenderer.CreateVertexBuffer();
		mParticleBuffers[n]->Store(p.data(), kMaxParticles * sizeof(Particle));

		mParticleFeedbacks[n]->SetDestination(mParticleBuffers[n]);
	}
}

void ParticleSystem::AdvanceFeedback()
{
	mRenderer.SetRasterizationState(true);
	UpdateEmitters();
	EmitParticles();
	ProcessParticles();
	mRenderer.SetRasterizationState(false);
}

void ParticleSystem::UpdateEmitters()
{
	if(!mEmitterBuffer)
		mEmitterBuffer = mRenderer.CreateVertexBuffer();
	mEmitterBuffer->Store(mEmitters, sizeof(mEmitters));
}

void ParticleSystem::EmitParticles()
{
	mEmitterFeedback->SetDestination(mEmitterFeedbackBuffer);

	const float emitCount = mParams.emitCount;
	mRenderer.UseProgram(mEmitterProgram);
	mEmitterProgram->SetUniform("u_EmitPeriod"_H, &mParams.emitPeriod);
	mEmitterProgram->SetUniform("u_EmitCount"_H, &emitCount);
	mEmitterProgram->SetUniform("u_DeltaTime"_H, &mDeltaTime);
	mEmitterProgram->SetUniform("u_Time"_H, &mTime);
	mEmitterProgram->SetUniform("u_RandomTexture"_H, &mRandomTexture);
	ApplyAttributes(mEmitterProgram, mEmitterBuffer, false);

	mEmitterFeedback->Begin(IndexBufferInfo::Mode::kPoints);
	mRenderer.Draw(IndexBufferInfo::Mode::kPoints, mEmittersCount);
	mEmitterFeedback->End();
}

void ParticleSystem::ProcessParticles()
{
	const int from = mCurrentBuffers % 2;
	const int to   = (mCurrentBuffers + 1) % 2;

	mParticleFeedbacks[to]->SetDestination(mParticleBuffers[to]);

	mRenderer.UseProgram(mFeedbackProgram);
	mFeedbackProgram->SetUniform("u_ParticleLifetime"_H, &mParams.particleLifetime);
	mFeedbackProgram->SetUniform("u_DeltaTime"_H, &mDeltaTime);
	mFeedbackProgram->SetUniform("u_Time"_H, &mTime);
	mFeedbackProgram->SetUniform("u_RandomTexture"_H, &mRandomTexture);
	mFeedbackProgram->SetUniform("u_FBMTexture"_H, &mFbmTexture);

	mParticleFeedbacks[to]->Begin(IndexBufferInfo::Mode::kPoints);

	//process newly generated particles from emitters feedback buffer
	ApplyAttributes(mFeedbackProgram, mEmitterFeedbackBuffer, false);

	mRenderer.Draw(mEmitterFeedback, IndexBufferInfo::Mode::kPoints);

	//feedback particles; kill the old ones
	if(mCurrentBuffers > 1)
	{
		ApplyAttributes(mFeedbackProgram, mParticleBuffers[from], false);
		mRenderer.Draw(mParticleFeedbacks[from], IndexBufferInfo::Mode::kPoints);
	}

	mParticleFeedbacks[to]->End();
}

void ParticleSystem::DrawParticles()
{
	Binding<Uniform<Matrix4>> viewMatrix("viewMatrix"_H, this);
	Binding<Uniform<Matrix4>> projectionMatrix("projectionMatrix"_H, this);
	Binding<Uniform<Matrix4>> modelMatrix("modelMatrix"_H, this);

	Matrix4 modelViewMatrix = **viewMatrix * **modelMatrix * Matrix4::Scale(10);
	Matrix4 mvp = **projectionMatrix * modelViewMatrix;
	Vector3 up = modelViewMatrix.GetRow(0).Xyz(); // up and right swapped here
	Vector3 right = modelViewMatrix.GetRow(1).Xyz();
	const float useColors = mParams.useColors ? 1.0f : 0.0f;

	mRenderer.UseProgram(mBillboardProgram);
	mBillboardProgram->SetUniform("u_MVP"_H, &mvp);
	mBillboardProgram->SetUniform("u_MV"_H, &modelViewMatrix);
	mBillboardProgram->SetUniform("u_Right"_H, &right);
	mBillboardProgram->SetUniform("u_Up"_H, &up);
	mBillboardProgram->SetUniform("u_BillboardSize"_H, &mParams.billboardSize);
	mBillboardProgram->SetUniform("u_VelocityScale"_H, &mParams.velocityScale);
	mBillboardProgram->SetUniform("u_ParticleLifetime"_H, &mParams.particleLifetime);
	mBillboardProgram->SetUniform("u_UseColors"_H, &useColors);
	mBillboardProgram->SetUniform("u_Texture"_H, &mParticleTexture);

	mRenderer.SetBlending(true, RenderCmdSink::kSrcAlpha, RenderCmdSink::kOne);
	mRenderer.SetDepthState(false, true);

	int ringEnd = (mCurrentBuffers + 1) % 2;
	ApplyAttributes(mBillboardProgram, mParticleBuffers[ringEnd], true);
	mRenderer.Draw(mParticleFeedbacks[ringEnd], IndexBufferInfo::Mode::kPoints);

	mRenderer.SetBlending(false);
	mRenderer.SetDepthState(true, true);
}

void ParticleSystem::MoveEmitters()
{
	for(int n = 0; n < mEmittersCount; n++)
	{
		Particle& p = mEmitters[n];
		if(!mParams.freezeEmitters)
		{
			p.position = Vector3(std::sin(mTime + 2 * 3.14f * p.color[0] / 255.f) * 0.2f,
									-1.5f,
									std::cos(mTime + 2 * 3.14f * p.color[1] / 255.f) * 0.2f);
		}
		if(p.age > mParams.emitPeriod)
			p.age = 0;
		else
			p.age += mDeltaTime;
	}
}

void ParticleSystem::Update(float deltaTime)
{
	MoveEmitters();

	mDeltaTime = deltaTime;
	mTime += mDeltaTime;
}

void ParticleSystem::SeedEmitters(unsigned int num)
{
	mEmittersCount = std::min(num, +kMaxEmitters);

	memset(mEmitters, 0, mEmittersCount * sizeof(Particle));
	for(int n = 0; n < mEmittersCount; n++)
	{
		mEmitters[n].color[0] = rand() & 255;
		mEmitters[n].color[1] = rand() & 255;
		mEmitters[n].color[2] = rand() & 255;
	}

	MoveEmitters();
}

void ParticleSystem::ApplyAttributes(RenderCmdSink::Program* program, RenderCmdSink::VertexBuffer* buffer, bool applyVelocity)
{
	program->SetAttribute("a_Color"_H, buffer, mVertexAttributeInfos[0]);
	program->SetAttribute("a_Age"_H, buffer, mVertexAttributeInfos[1]);
	program->SetAttribute("a_Position"_H, buffer, mVertexAttributeInfos[2]);
	if(applyVelocity)
		program->SetAttribute("a_Velocity"_H, buffer, mVertexAttributeInfos[3]);
}

}
}
