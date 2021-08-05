#include "NITimeController.h"

namespace NI {
	TimeController_vTable::TimeController_vTable() {
		// Copy everything from the original virtual table.
		memcpy_s(this, sizeof(TimeController_vTable), (void*)0x751200, sizeof(TimeController_vTable));
	}

	void TimeController::ctor() {
		_ctor(this);
	}

	void TimeController::dtor() {
		_dtor(this);
	}

	void TimeController::start(float time) {
		vTable.asController->start(this, time);
	}

	void TimeController::stop() {
		vTable.asController->stop(this);
	}

	void TimeController::update(float dt) {
		vTable.asController->update(this, dt);
	}

	void TimeController::setTarget(ObjectNET* target) {
		vTable.asController->setTarget(this, target);
	}

	float TimeController::computeScaledTime(float dt) {
		return vTable.asController->computeScaledTime(this, dt);
	}

	bool TimeController::targetIsRequiredType() const {
		return vTable.asController->targetIsRequiredType(this);
	}

	bool TimeController::getActive() const {
		return flags & TimeControllerFlags::Active;
	}

	void TimeController::setActive(bool active) {
		if (active) {
			flags |= TimeControllerFlags::Active;
		}
		else {
			flags &= ~TimeControllerFlags::Active;
		}
	}

	unsigned int TimeController::getAnimTimingType() const {
		return flags & TimeControllerFlags::AppTimingMask;
	}

	void TimeController::setAnimTimingType(unsigned int type) {
		flags = (flags & ~TimeControllerFlags::AppTimingMask) | (type & TimeControllerFlags::AppTimingMask);
	}

	unsigned int TimeController::getCycleType() const {
		return flags & TimeControllerFlags::CycleTypeMask;
	}

	void TimeController::setCycleType(unsigned int type) {
		flags = (flags & ~TimeControllerFlags::CycleTypeMask) | (type & TimeControllerFlags::CycleTypeMask);
	}

}

MWSE_SOL_CUSTOMIZED_PUSHER_DEFINE_NI(NI::TimeController)
