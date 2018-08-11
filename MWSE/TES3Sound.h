#pragma once

#include "TES3Defines.h"

#include "TES3Object.h"

namespace TES3 {
	namespace SoundPlayFlags {
		typedef int value_type;

		enum Flag {
			Loop = 1
		};
	}

	struct SoundBuffer {
		void* lpSoundBuffer;
		void* lpSound3DBuffer;
		char fileHeader[16];
		short unknown_0x18;
		int unknown_0x1C;
		int flags;
		int waveSize;
		int unknown_0x28;
		unsigned char* wavHeader;
		int unknown_0x30;
		int unknown_0x34;
		int unknown_0x38;
		int unknown_0x3C;
		bool isVoiceover;
		short* rawAudio;
		int volume_related_0x48;
		unsigned char volume;
		int minDistance;
		int maxDistance;
	};
	static_assert(sizeof(SoundBuffer) == 0x58, "TES3::SoundBuffer failed size validation");

	struct Sound : BaseObject {
		char field_10;
		char id[32];
		char filename[32];
		unsigned char volume;
		unsigned char minDistance;
		unsigned char maxDistance;
		SoundBuffer* soundBuffer;

		//
		// Other related this-call functions.
		//

		bool play(int playbackFlags = 0, unsigned char volume = 250, float pitch = 1.0f, bool isNot3D = true);

	};
	static_assert(sizeof(Sound) == 0x58, "TES3::Sound failed size validation");
}
