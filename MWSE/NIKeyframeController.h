#pragma once

#include "NITimeController.h"
#include "TES3Vectors.h"

namespace NI {
	enum class AnimationKeyType : unsigned int {
		NoInterp = 0,
		Linear = 1,
		Bezier = 2,
		TCB = 3,
		Euler = 4,
		NumKeyTypes = 5
	};

	struct AnimationKey {
		float timing; // 0x0
		unsigned char size; // 0x4
	};
	static_assert(sizeof(AnimationKey) == 0x8, "TES3::AnimationKey failed size validation");

	struct FloatKey : AnimationKey {
		float value; // 0x8
	};
	static_assert(sizeof(FloatKey) == 0xC, "TES3::FloatKey failed size validation");

	struct KeyframeData : Object {
		unsigned int rotationKeyCount; // 0x8
		AnimationKey* rotationKeys; // 0xC
		AnimationKeyType rotationType; // 0x10
		unsigned int positionKeyCount; // 0x14
		AnimationKey* positionKeys; // 0x18
		AnimationKeyType positionType; // 0x1C
		unsigned int scaleKeyCount; // 0x20
		AnimationKey* scaleKeys; // 0x24
		AnimationKeyType scaleType; // 0x28
	};
	static_assert(sizeof(KeyframeData) == 0x2C, "TES3::KeyframeData failed size validation");

	struct KeyframeController : TimeController {
		unsigned int lastRotIndex; // 0x34
		unsigned int lastPosIndex; // 0x38
		unsigned int lastScaleIndex; // 0x3C
		Pointer<KeyframeData> keyframeData; // 0x40
	};
	static_assert(sizeof(KeyframeController) == 0x44, "TES3::KeyframeController failed size validation");
}
