#include "TES3AudioController.h"
#include "TES3Sound.h"
#include "TES3Util.h"
#include "TES3WorldController.h"

namespace TES3 {
	const auto TES3_Sound_play = reinterpret_cast<bool (__thiscall *)(Sound*, int, unsigned char, float, bool)>(0x510A40);

	bool Sound::play(int playbackFlags, unsigned char volume, float pitch, bool isNot3D) {
		unsigned int master = mwse::tes3::getWorldController()->audioController->volumeMaster;
		unsigned int finalVolume = master * volume / 250;
		return TES3_Sound_play(this, playbackFlags, finalVolume, pitch, isNot3D);
	}
}
