#pragma once

#include "TES3Defines.h"

#include "TES3Collections.h"

#include <Windows.h>

namespace TES3 {
	struct Object_0x751248 {
		int unknown_0x0;
		int unknown_0x4;
		int unknown_0x8;
		int unknown_0xC;
		int unknown_0x10;
		int unknown_0x14;
	};
	static_assert(sizeof(Object_0x751248) == 0x18, "TES3::Object_0x751248 failed size validation");

	struct MobController_0x0 {
		Object_0x751248 unknown_0x0;
		Object_0x751248 unknown_0x18;
	};
	static_assert(sizeof(MobController_0x0) == 0x30, "TES3::MobController_0x0 failed size validation");

	struct MobController_0x24 {
		MobilePlayer * mobilePlayer; // 0x0
		Iterator<void> unknown_0x4;
		bool unknown_0x18;
		bool unknown_0x19; // Maybe force new actions?
		_RTL_CRITICAL_SECTION criticalSection;
		unsigned char unknown[0x7FC];

		//
		// Related this-call functions.
		//

		bool detectPresence(MobileActor * actor, bool unknown = true);
		void checkRadius(MobileActor * actor, Iterator<void> * container);
		void checkPlayerDistance();

	};
	static_assert(sizeof(MobController_0x24) == 0x830, "TES3::MobController_0x24 failed size validation");

	struct MobController_0x28 {
		Iterator<void> unknown_0x0;
		int unknown_0x18;
		_RTL_CRITICAL_SECTION criticalSection;
		int unknown_0x30;
		int unknown_0x34;
		int unknown_0x38;
	};
	static_assert(sizeof(MobController_0x28) == 0x3C, "TES3::MobController_0x28 failed size validation");

	struct MobController {
		MobController_0x0 * unknown_0x0;
		int unknown_0x4;
		int unknown_0x8;
		float gravity; // 0xC
		int unknown_0x10;
		int unknown_0x14;
		float unknown_0x18; // Initialized to -4005.5
		float unknown_0x1C; // Initialized to 46 degrees.
		float unknown_0x20; // Initialized to cos(46 degrees)
		MobController_0x24 * unknown_0x24; // Mobile actor controller?
		MobController_0x28 * unknown_0x28;
		bool unknown_0x2C;
		Iterator<void> unknown_0x30;
		int unknown_0x44;
		int unknown_0x48;
		int unknown_0x4C;
		int unknown_0x50;
		int unknown_0x54;
		int unknown_0x58;
		int unknown_0x5C;
		int unknown_0x60;
		int unknown_0x64;
		int unknown_0x68;
		int unknown_0x6C;
		int unknown_0x70;
		int unknown_0x74;
		int unknown_0x78;
		int unknown_0x7C;
		int unknown_0x80;
		int unknown_0x84;
		int unknown_0x88;

		//
		// Related this-call functions.
		//

		void checkPlayerDistance();

	};
	static_assert(sizeof(MobController) == 0x8C, "TES3::MobController failed size validation");
}
