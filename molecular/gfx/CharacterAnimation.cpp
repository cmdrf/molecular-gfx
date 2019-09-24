/*	CharacterAnimation.cpp

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

#include "CharacterAnimation.h"

namespace molecular
{
namespace CharacterAnimation
{

const int kBoneParents[80] =
{
    -1, // root (0)
    0, // pelvis (1)
    1, // spine01 (2)
    2, // spine02 (3)
    3, // spine03
    4, // spine04
    5, // neck (6)
    6, // head (7)
    7, // lookIK (8)
    7, // lookIK_start (9)
    5, // L_clavicle (10)
    10, // L_upperarm
    11, // L_forearm (12)
    12, // L_hand (13)
    13, // L_thumb_01
    14, // L_thumb_02
    15, // L_thumb_03 (16)
    16, // L_thumb_end
    13, // L_index_01 (18)
    18, // L_index_02
    19, // L_index_03
    20, // L_index_end
    13, // L_middle_01 (22)
    22, // L_middle_02
    23, // L_middle_03
    24, // L_middle_end (25)
    13, // L_ring_01 (26)
    26, // L_ring_02
    27, // L_ring_03 (28)
    28, // L_ring_end (29)
    13, // L_pinky_01 (30)
    30, // L_pinky_02
    31, // L_pinky_03
    32, // L_pinky_end (33)
    13, // RT_weapon_L_default_target (34)
    12, // L_forearm_twist
    10, // L_upperarm_twist (36)
    5, // R_clavicle (37)
    37, // R_upperarm (38)
    38, // R_forearm (39)
    39, // R_hand (40)
    40, // R_thumb_01
    41, // R_thumb_02
    42, // R_thumb_03
    43, // R_thumb_04 (44)
    40, // R_index_01 (45)
    45, // R_index_02
    46, // R_index_03
    47, // R_index_04
    40, // R_middle_01 (49)
    49, // R_middle_02 (50)
    50, // R_middle_03
    51, // R_middle_04
    40, // R_ring_01 (53)
    53, // R_ring_02
    54, // R_ring_03
    55, // R_ring_04
    40, // R_pinky_01 (57)
    57, // R_pinky_02 (58)
    58, // R_pinky_03 (59)
    59, // R_pinky_04 (60)
    40, // RT_weapon_R_default_target (61)
    39, // R_forearm_twist (62)
    38, // R_upperarm_twist (63)
    1, // L_thigh (64)
    64, // L_calf
    65, // L_foot (66)
    66, // L_toe
    67, // L_toe_end (68)
    66, // planeTargetLeft (69)
    66, // L_Heel (70)
    64, // L_thigh_twist (71)
    1, // R_thigh (72)
    72, // R_calf (73)
    73, // R_foot (74)
    74, // R_toe
    75, // R_toe_end
    74, // planeTargetRight (77)
    74, // R_Heel (78)
    72 // R_thigh_twist (79)
};

const Hash kBoneNameToIndex[] =
{
	"root"_H,
	"pelvis"_H,
	"spine01"_H,
	"spine02"_H,
	"spine03"_H,
	"spine04"_H,
	"neck"_H,
	"head"_H,
	"lookIK"_H,
	"lookIK_start"_H,
	"L_clavicle"_H,
	"L_upperarm"_H,
	"L_forearm"_H,
	"L_hand"_H,
	"L_thumb_01"_H,
	"L_thumb_02"_H,
	"L_thumb_03"_H,
	"L_thumb_end"_H,
	"L_index_01"_H,
	"L_index_02"_H,
	"L_index_03"_H,
	"L_index_end"_H,
	"L_middle_01"_H,
	"L_middle_02"_H,
	"L_middle_03"_H,
	"L_middle_end"_H,
	"L_ring_01"_H,
	"L_ring_02"_H,
	"L_ring_03"_H,
	"L_ring_end"_H,
	"L_pinky_01"_H,
	"L_pinky_02"_H,
	"L_pinky_03"_H,
	"L_pinky_end"_H,
	"RT_weapon_L_default_target"_H,
	"L_forearm_twist"_H,
	"L_upperarm_twist"_H,
	"R_clavicle"_H,
	"R_upperarm"_H,
	"R_forearm"_H,
	"R_hand"_H,
	"R_thumb_01"_H,
	"R_thumb_02"_H,
	"R_thumb_03"_H,
	"R_thumb_04"_H,
	"R_index_01"_H,
	"R_index_02"_H,
	"R_index_03"_H,
	"R_index_04"_H,
	"R_middle_01"_H,
	"R_middle_02"_H,
	"R_middle_03"_H,
	"R_middle_04"_H,
	"R_ring_01"_H,
	"R_ring_02"_H,
	"R_ring_03"_H,
	"R_ring_04"_H,
	"R_pinky_01"_H,
	"R_pinky_02"_H,
	"R_pinky_03"_H,
	"R_pinky_04"_H,
	"RT_weapon_R_default_target"_H,
	"R_forearm_twist"_H,
	"R_upperarm_twist"_H,
	"L_thigh"_H,
	"L_calf"_H,
	"L_foot"_H,
	"L_toe"_H,
	"L_toe_end"_H,
	"planeTargetLeft"_H,
	"L_Heel"_H,
	"L_thigh_twist"_H,
	"R_thigh"_H,
	"R_calf"_H,
	"R_foot"_H,
	"R_toe"_H,
	"R_toe_end"_H,
	"planeTargetRight"_H,
	"R_Heel"_H,
	"R_thigh_twist"_H
};

template<typename T>
T Lerp(const T& v0, const T& v1, float s)
{
	return v0 * (1.0f - s) + v1 * s;
}

Sqt Interpolate(const Sqt& prev, const Sqt& next, float t)
{
	assert(t <= 1.0f);
	assert(t >= 0.0f);
	CharacterAnimation::Sqt out;
	out.translation = Lerp(prev.translation, next.translation, t);
	out.scale = Lerp(prev.scale, next.scale, t);
	out.rotation = Quaternion::Slerp(prev.rotation, next.rotation, t);
//	out.rotation = Lerp(prev.rotation, next.rotation, t);
	return out;
}

int GetBoneIndex(Hash hash)
{
	for(int i = 0; i < sizeof(kBoneNameToIndex); i++)
	{
		if(kBoneNameToIndex[i] == hash)
			return i;
	}
	return -1;
}

void SceneGraphToAbsoluteTransforms(const Matrix4 graph[], const int parents[], Matrix4 output[], int count)
{
	for(int i = 0; i < count; i++)
	{
		int parent = parents[i];
		assert(parent < i);
		if(parent < 0)
			output[i] = graph[i];
		else
			output[i] = output[parent] * graph[i];
	}
}

}
}
