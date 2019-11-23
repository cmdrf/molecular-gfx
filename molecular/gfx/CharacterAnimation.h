/*	CharacterAnimation.h

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

#ifndef MOLECULAR_CHARACTERANIMATION_H
#define MOLECULAR_CHARACTERANIMATION_H

#include <cstdint>
#include <molecular/util/Matrix3.h>
#include <molecular/util/Matrix4.h>
#include <molecular/util/Hash.h>
#include <molecular/util/Quaternion.h>

namespace molecular
{
namespace gfx
{
namespace CharacterAnimation
{
using namespace util;

constexpr int kBoneCount = 80;

/// Parent indices for each bone
/** Root bone has -1 as parent. */
extern const int kBoneParents[kBoneCount];

/// Scale, quaternion, translation
struct Sqt
{
	Quaternion rotation;
	Vector3 translation;
	float scale;

	Matrix4 ToMatrix4() const {return Matrix4::Translation(translation) * Matrix4(Matrix3(rotation) * Matrix3::Scale(scale));}
};

Sqt Interpolate(const Sqt& prev, const Sqt& next, float t);

struct File
{
	struct Joint
	{
		uint32_t numSamples;
		uint32_t inputOffset;
		uint32_t outputOffset;
		uint32_t reserved;
	};

	static const uint32_t kMagic = 0x80a518a7;
	static const uint32_t kVersion = 3;
	static const int kNumJoints = kBoneCount;
	uint32_t magic;
	uint32_t version;
	uint32_t name;
	float duration; ///< Animation duration in seconds
	Joint joints[kNumJoints];

	const float* GetInput(unsigned int i) const
	{
		assert(i < kNumJoints);
		return reinterpret_cast<const float*>(reinterpret_cast<const char*>(this) + joints[i].inputOffset);
	}

	const Sqt* GetOutput(unsigned int i) const
	{
		assert(i < kNumJoints);
		return reinterpret_cast<const Sqt*>(reinterpret_cast<const char*>(this) + joints[i].outputOffset);
	}
};

/// File structure for storing inverse bind pose matrices
struct BindPoseFile
{
	static constexpr uint32_t kMagic = 0x80b1b0f1;
	static constexpr uint32_t kVersion = 1;

	uint32_t magic;
	uint32_t version;
	Matrix4 inverseBindPoseMatrices[kBoneCount];
};

/// Get bone index for a hashed bone name
int GetBoneIndex(Hash hash);

void SceneGraphToAbsoluteTransforms(const Matrix4 graph[], const int parents[], Matrix4 output[], int count);

} // CharacterAnimation
} // gfx
} // molecular

#endif
