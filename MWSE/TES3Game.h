#pragma once

#include "NIDefines.h"
#include "TES3Defines.h"

#define NOMINMAX
#include <Windows.h>

namespace TES3 {
	struct Game {
		void * vTable;
		void * unknown_0x4;
		int unknown_0x8;
		int unknown_0xC;
		int unknown_0x10;
		int unknown_0x14;
		int unknown_0x18;
		char unknown_0x1C;
		char unknown_0x1D;
		char unknown_0x1E;
		char unknown_0x1F;
		char unknown_0x20;
		char unknown_0x21;
		bool screenShotsEnabled; // 0x22
		int unknown_0x24;
		int unknown_0x28;
		int unknown_0x2C;
		int unknown_0x30;
		int unknown_0x34;
		int unknown_0x38;
		float unknown_0x3C;
		int unknown_0x40;
		int screenX; // 0x44
		int screenY; // 0x48
		float renderDistance; // 0x4C
		unsigned char volumeMaster; // 0x50
		unsigned char volumeVoice; // 0x51
		unsigned char volumeEffect; // 0x52
		unsigned char volumeFootsteps; // 0x53
		int volumeMedia; // 0x54
		unsigned char soundQuality; // 0x58
		char unknown_0x59;
		char unknown_0x5A;
		char unknown_0x5B;
		HWND parentWindowHandle; // 0x5C
		HWND windowHandle; // 0x60
		void * sgNode1;
		int unknown_0x68;
		int unknown_0x6C;
		int unknown_0x70;
		char unknown_0x74;
		char unknown_0x75;
		void * unknown_0x78;
		char unknown_0x7C;
		int unknown_0x80;
		int unknown_0x84;
		void * unknown_0x88; // std::string?
		int unknown_0x8C;
		int unknown_0x90;
		int unknown_0x94;
		void * unknown_0x98; // List?
		NI::Node * worldRoot; // 0x9C
		int unknown_0xA0;
		int unknown_0xA4;
		int unknown_0xA8;
		int unknown_0xAC;
		int unknown_0xB0;
		int unknown_0xB4;
		int unknown_0xB8;
		int unknown_0xBC;
		int unknown_0xC0;
		int unknown_0xC4;
		int unknown_0xC8;
		int unknown_0xCC;
		void * unknown_0xD0[3]; // SG nodes?
		int unknown_0xDC;
		int unknown_0xE0;
		int unknown_0xE4;
		Reference * playerTarget; // 0xE8
		int unknown_0xEC;
		int unknown_0xF0;
		int unknown_0xF4;
		int unknown_0xF8;
		int unknown_0xFC;
		int unknown_0x100;
		int unknown_0x104;
		int unknown_0x108;
		char unknown_0x10C;
		char unknown_0x10D[3]; // Padding.

		//
		// Other related this-call functions.
		//

		bool initialize();

	};
	static_assert(sizeof(Game) == 0x110, "TES3::Game failed size validation");
}
