#pragma once

#include "NIAVObject.h"
#include "NITimeController.h"

namespace NI {
	namespace LookAtControllerFlags {
		typedef TimeControllerFlags::value_type value_type;

		enum LookAtControllerFlags {
			FlipAxis = 0x10,
			AxisX = 0,
			AxisY = 0x20,
			AxisZ = 0x40,
			AxisMask = 0x60
		};

		enum LookAtControllerFlagBit {
			FlipAxisBit = 4,
			AxisBit = 5
		};
	}

	struct LookAtController : TimeController {
		AVObject* lookAt; // 0x34. Is not an NI::Pointer.

		//
		// Other related this-call functions.
		//

		unsigned int getAxis() const;
		void setAxis(unsigned int axis);
		bool getFlip() const;
		void setFlip(bool flip);
		AVObject * getLookAt() const;
		void setLookAt(AVObject* lookAtObject);
	};
	static_assert(sizeof(LookAtController) == 0x38, "NI::LookAtController failed size validation");
}

MWSE_SOL_CUSTOMIZED_PUSHER_DECLARE_NI(NI::LookAtController)