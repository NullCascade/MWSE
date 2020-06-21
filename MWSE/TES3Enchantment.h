#pragma once

#include "TES3Defines.h"

#include "TES3MagicEffect.h"
#include "TES3Object.h"

namespace TES3 {
	enum class EnchantmentCastType : unsigned short {
		Once,
		OnStrike,
		OnUse,
		Constant,
		Invalid
	};

	struct Enchantment : Object {
		Enchantment();
		~Enchantment();

		char * objectID; // 0x28
		EnchantmentCastType castType; // 0x2C
		unsigned short chargeCost; // 0x2E
		unsigned short maxCharge; // 0x30
		short pad_32; // 0x32
		Effect effects[8]; // 0x34
		unsigned int flags; // 0xF4

		//
		// Custom functions.
		//

		size_t getActiveEffectCount();
		int getFirstIndexOfEffect(int effectId);

		std::reference_wrapper<Effect[8]> getEffects();

	};
	static_assert(sizeof(Enchantment) == 0xF8, "TES3::Enchantment failed size validation");
}
