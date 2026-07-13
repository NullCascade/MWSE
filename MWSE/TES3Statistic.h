#pragma once

#include "TES3Defines.h"

namespace TES3 {
	struct Statistic_vTable {
		float (__thiscall * getCurrentValue)(const Statistic*);
	};

	struct Statistic {
		Statistic_vTable * vTable;
		float base;
		float current;

		Statistic() = delete;
		~Statistic() = delete;

		//
		// Virtual table function wrappers.
		//

		float getCurrent() const;

		//
		// Other related this-call functions.
		//

		float getBase() const;
		float getCurrentRaw() const;
		float getNormalized() const;

		void modBaseCapped(float delta, bool capAt0, bool capAt100);
		void modCurrentCapped(float delta, bool capAt0, bool capAtBase, bool capAt100);

		void setBase(float value);
		void setBaseAndCurrent(float value);
		void setBaseToCurrent();
		void setCurrentCapped(float value, bool applyCaps);

		//
		// Custom functions.
		//

		void setCurrent_lua(float value);
		float getOpenMWModifier() const;
		float getOpenMWDamage() const;
		float getOpenMWModified() const;
		void setOpenMWBase(float value);
		void setOpenMWModifier(float value);
		void setOpenMWDamage(float value);

	};
	static_assert(sizeof(Statistic) == 0xC, "TES3::Statistic failed size validation");


	struct SkillStatistic : Statistic {
		unsigned int type; // 0xC // Values from TES3::SkillType::SkillType enumeration

		SkillStatistic() = delete;
		~SkillStatistic() = delete;

		//
		// Other related this-call functions.
		//

		void modSkillCapped(float delta, bool capAt0 = false, bool capAt100 = false);

	};
	static_assert(sizeof(SkillStatistic) == 0x10, "TES3::SkillStatistic failed size validation");
}
