#pragma once

#include "TES3Defines.h"
#include "TES3Vectors.h"

namespace TES3 {
	enum class AudioMixType {
		Master = 0,
		Voice,
		Effects,
		Footsteps,
		Music
	};

	struct AudioController
	{
		char field_0;
		char field_1;
		int flags_4;
		int flags_8;
		void* lpDirectSound;
		void* lpPrimaryBuffer;
		void* lpListener3D;
		char field_18;
		int dsoundCaps;
		int field_20;
		int field_24;
		int field_28;
		int field_2C;
		int field_30;
		int field_34;
		int field_38;
		int field_3C;
		int field_40;
		int field_44;
		int field_48;
		int field_4C;
		int field_50;
		int field_54;
		int field_58;
		int field_5C;
		int field_60;
		int field_64;
		int field_68;
		int field_6C;
		int field_70;
		int field_74;
		int field_78;
		char nextMusicFilePath[260];
		char currentMusicFilePath[260];
		int timestampBeginFade;
		int timestampNextTrackStart;
		int volumeNextTrack;
		unsigned char volumeMaster;
		float volumeMusic;
		unsigned char volumeEffects;
		unsigned char volumeVoice;
		unsigned char volumeFootsteps;
		int field_29C;
		void* lpDsoundBufferMusic;
		void* lpDsoundBufferMusic2;
		bool flagDisableAudio;
		Vector3 listenerPosition;
		Vector3 orientation_2B8;
		Vector3 orientation_2C4;
		float radiansYawAxis;
		float radiansPitchAxisApproximated;

		//
		// Thiscall functions.
		//

		float getMixVolume(AudioMixType);
		void changeMusicTrack(const char* filename, int crossfadeMillis, float volume);
	};
	static_assert(sizeof(AudioController) == 0x2D8, "TES3::AudioController failed size validation");
}