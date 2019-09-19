/*	CpuParticleSystem.h
	Copyright 2016-2017 Fabian Herb

	This file is part of Molecular Engine.
*/

#ifndef CPUPARTICLESYSTEM_H
#define CPUPARTICLESYSTEM_H

#include "DrawingFunction.h"
#include <Eigen/Dense>
#include <Eigen/StdVector>

namespace Gfx
{

struct ParticleSystemParams
{
	Vector3 wind;
	Vector3 acceleration;
	Vector3 emissionPositionRandomness;
	Vector3 emissionVelocityRandomness;
	Vector3 transitionPositionRandomness;
	Vector3 transitionVelocityRandomness;
	Vector3 color;
	float emitsPerSecond;
	float maxParticleAge;
	bool castShadows;
};

template<class TRenderManager>
class CpuParticleSystem : public DrawingFunction
{
public:
	using RenderManager = TRenderManager;
	using TaskQueue = typename RenderManager::TaskQueue;

	CpuParticleSystem(TRenderManager& manager);
	~CpuParticleSystem();

	void Execute() override;
	AxisAlignedBox GetBounds() const override {return AxisAlignedBox();}

	/// Set emitters
	/** Replaces all currently set emitters. Sets position only.
		@todo Set velocity. */
	template<typename Iterator>
	void SetEmitters(Iterator begin, Iterator end);

	void SetWind(Vector3 wind);

	/// Set constant acceleration applied to all particles
	/** E.g. gravity. */
	void SetAcceleration(Vector3 acceleration);

	/// Set number of emitted particles per emitter per second
	void SetEmitsPerSecond(float emitsPerSecond) {mEmitsPerSecond = emitsPerSecond;}

	void SetMaxParticleAge(float maxParticleAge) {mMaxParticleAge = maxParticleAge;}

//	void SetEmissionRandomness(Vector3 positionRandomness, Vector3 velocityRandomness);
//	void SetTransitionRandomness(Vector3 positionRandomness, Vector3 velocityRandomness);

private:
	enum
	{
		kIndexPosX = 0,
		kIndexPosY,
		kIndexPosZ,
		kIndexVelX,
		kIndexVelY,
		kIndexVelZ,
		kIndexAge,
		kIndexReserved
	};
	/** posX, posY, posZ, velX, velY, velZ, age, reserved. */
	using ParticleVec = Eigen::Matrix<float, 8, 1>;
	using ParticleVecContainer = std::vector<ParticleVec, Eigen::aligned_allocator<ParticleVec>>;

	void RemoveOld(float maxParticleAge);
	void Transition(float t, ParticleVec randomSigma, ParticleVec additive);
	void Emission(float t, 	ParticleVec randomSigma, float emitsPerSecond);

	/** Written by async simulation task. Copied to GPU after task finishes. */
	ParticleVecContainer mParticles;

	/** Written by accessor methods like SetEmitters(). Copied to mSimulationTaskEmitters before
		task starts.*/
	ParticleVecContainer mEmitters;

	/** Read by simulation task. Set to mEmitters before task starts. */
	ParticleVecContainer mSimulationTaskEmitters;

	/// Number of emitted particles per emitter per second
	float mEmitsPerSecond = 800;
	float mMaxParticleAge = 1.0;

	/** windX, windY, windZ, accX, accY, accZ, 1, reserved */
	ParticleVec mAdditive;

	ParticleVec mTransitionRandomSigma;
	ParticleVec mEmissionRandomSigma;
	bool mCastShadows = false;
	Vector3 mColor = Vector3(0.2f, 0.035f, 0.005f);
	static const int kMaxParticles = 4096;

	RenderCmdSink::VertexBuffer* mVertexBuffer;
	VertexAttributeInfo mPositionInfo;
	VertexAttributeInfo mAgeInfo;

	TaskQueue& mTaskQueue;
	typename TaskQueue::FinishFlag mSimulationFinishFlag;
	size_t mNumParticles = 0;
};

}

#endif // CPUPARTICLESYSTEM_H
