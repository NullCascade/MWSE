#include <cstdio>

#include "TES3AudioController.h"

namespace TES3 {
	const auto TES3_AudioController_changeMusicTrack = reinterpret_cast<void(__thiscall*)(AudioController*, const char*, int, float)>(0x403AC0);

	float AudioController::getMixVolume(AudioMixType mix) {
		float volume = 0.004 * volumeMaster;
		switch (mix) {
		case AudioMixType::Master:
			break;
		case AudioMixType::Voice:
			volume *= 0.004 * volumeVoice;
			break;
		case AudioMixType::Effects:
			volume *= 0.004 * volumeEffects;
			break;
		case AudioMixType::Footsteps:
			volume *= 0.004 * volumeFootsteps;
			break;
		case AudioMixType::Music:
			// Music is not linked to master volume
			volume = volumeMusic;
			break;
		}
		return volume;
	}

	void AudioController::changeMusicTrack(const char* filename, int crossfadeMillis, float volume) {
		TES3_AudioController_changeMusicTrack(this, filename, crossfadeMillis, volume);
	}

}