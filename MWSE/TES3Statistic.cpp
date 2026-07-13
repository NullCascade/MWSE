#include "TES3Statistic.h"

namespace TES3 {
	//
	// Virtual table function wrappers.
	//

	float Statistic::getCurrent() const {
		return vTable->getCurrentValue(this);
	}

	//
	// Other related this-call functions.
	//

	const auto TES3_Statistic_getBase = reinterpret_cast<float(__thiscall*)(const Statistic*)>(0x401030);
	float Statistic::getBase() const {
		return TES3_Statistic_getBase(this);
	}

	const auto TES3_Statistic_getCurrent = reinterpret_cast<float(__thiscall*)(const Statistic*)>(0x40D3B0);
	float Statistic::getCurrentRaw() const {
		return TES3_Statistic_getCurrent(this);
	}

	const auto TES3_Statistic_getNormalized = reinterpret_cast<float(__thiscall*)(const Statistic*)>(0x4012B0);
	float Statistic::getNormalized() const {
		return TES3_Statistic_getNormalized(this);
	}

	const auto TES3_Statistic_modBaseWithCaps = reinterpret_cast<float(__thiscall*)(Statistic*, float, bool, bool)>(0x401100);
	void Statistic::modBaseCapped(float delta, bool capAt0, bool capAt100) {
		TES3_Statistic_modBaseWithCaps(this, delta, capAt0, capAt100);
	}

	const auto TES3_Statistic_modCurrentWithCaps = reinterpret_cast<float(__thiscall*)(Statistic*, float, bool, bool, bool)>(0x401150);
	void Statistic::modCurrentCapped(float delta, bool capAt0, bool capAtBase, bool capAt100) {
		TES3_Statistic_modCurrentWithCaps(this, delta, capAt0, capAtBase, capAt100);
	}

	const auto TES3_Statistic_setBase = reinterpret_cast<float(__thiscall*)(Statistic*, float)>(0x401240);
	void Statistic::setBase(float value) {
		TES3_Statistic_setBase(this, value);
	}

	const auto TES3_Statistic_setBaseAndCurrent = reinterpret_cast<float(__thiscall*)(Statistic*, float)>(0x401290);
	void Statistic::setBaseAndCurrent(float value) {
		TES3_Statistic_setBaseAndCurrent(this, value);
	}

	const auto TES3_Statistic_setBaseToCurrent = reinterpret_cast<float(__thiscall*)(Statistic*)>(0x4012A0);
	void Statistic::setBaseToCurrent() {
		TES3_Statistic_setBaseToCurrent(this);
	}

	const auto TES3_Statistic_setCurrentCapped = reinterpret_cast<float(__thiscall*)(Statistic*, float, bool)>(0x401250);
	void Statistic::setCurrentCapped(float value, bool applyCaps) {
		TES3_Statistic_setCurrentCapped(this, value, applyCaps);
	}

	void Statistic::setCurrent_lua(float value) {
		setCurrentCapped(value, false);
	}

	float Statistic::getOpenMWModifier() const {
		return std::max(0.0f, getCurrentRaw() - getBase());
	}

	float Statistic::getOpenMWDamage() const {
		return std::max(0.0f, getBase() - getCurrentRaw());
	}

	float Statistic::getOpenMWModified() const {
		return std::max(0.0f, getBase() - getOpenMWDamage() + getOpenMWModifier());
	}

	void Statistic::setOpenMWBase(float value) {
		const float difference = getCurrentRaw() - getBase();
		setBase(value);
		setCurrentCapped(value + difference, false);
	}

	void Statistic::setOpenMWModifier(float value) {
		setCurrentCapped(getBase() - getOpenMWDamage() + value, false);
	}

	void Statistic::setOpenMWDamage(float value) {
		setCurrentCapped(getBase() - value + getOpenMWModifier(), false);
	}

	const auto TES3_SkillStatistic_modSkillCapped = reinterpret_cast<void(__thiscall*)(SkillStatistic*, float, bool, bool)>(0x401060);
	void SkillStatistic::modSkillCapped(float delta, bool capAt0, bool capAt100) {
		TES3_SkillStatistic_modSkillCapped(this, delta, capAt0, capAt100);
	}
}
