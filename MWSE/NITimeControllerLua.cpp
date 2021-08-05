#include "NITimeControllerLua.h"

#include "NIObjectLua.h"

#include "LuaManager.h"
#include "LuaUtil.h"

#include "NILookAtController.h"
#include "NITimeController.h"

namespace mwse {
	namespace lua {
		template <typename T>
		void setUserdataForNITimeController(sol::usertype<T>& usertypeDefinition) {
			setUserdataForNIObject(usertypeDefinition);

			// Basic property binding.
			usertypeDefinition["frequency"] = &NI::TimeController::frequency;
			usertypeDefinition["phase"] = &NI::TimeController::phase;
			usertypeDefinition["lowKeyFrame"] = &NI::TimeController::lowKeyFrame;
			usertypeDefinition["highKeyFrame"] = &NI::TimeController::highKeyFrame;
			usertypeDefinition["startTime"] = &NI::TimeController::startTime;
			usertypeDefinition["lastScaledTime"] = &NI::TimeController::lastScaledTime;
			usertypeDefinition["lastTime"] = &NI::TimeController::lastTime;
			usertypeDefinition["target"] = sol::readonly_property(&NI::TimeController::target);
			usertypeDefinition["nextController"] = &NI::TimeController::nextController;
			usertypeDefinition["active"] = sol::property(&NI::TimeController::getActive, &NI::TimeController::setActive);
			usertypeDefinition["animTimingType"] = sol::property(&NI::TimeController::getAnimTimingType, &NI::TimeController::setAnimTimingType);
			usertypeDefinition["cycleType"] = sol::property(&NI::TimeController::getCycleType, &NI::TimeController::setCycleType);

			// Basic function binding.
			usertypeDefinition["setTarget"] = &NI::TimeController::setTarget;
			usertypeDefinition["start"] = &NI::TimeController::start;
			usertypeDefinition["stop"] = &NI::TimeController::stop;
		}

		void bindNITimeController() {
			// Get our lua state.
			auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
			sol::state& state = stateHandle.state;

			// Bind NI::TimeController
			{
				// Start our usertype. We must finish this with state.set_usertype.
				auto usertypeDefinition = state.new_usertype<NI::TimeController>("niTimeController");
				usertypeDefinition["new"] = sol::no_constructor;

				// Define inheritance structures. These must be defined in order from top to bottom. The complete chain must be defined.
				usertypeDefinition[sol::base_classes] = sol::bases<NI::Object>();
				setUserdataForNITimeController(usertypeDefinition);
			}

			// Bind NI::LookAtController
			{
				// Start our usertype. We must finish this with state.set_usertype.
				auto usertypeDefinition = state.new_usertype<NI::LookAtController>("niLookAtController");
				usertypeDefinition["new"] = sol::no_constructor;

				// Define inheritance structures. These must be defined in order from top to bottom. The complete chain must be defined.
				usertypeDefinition[sol::base_classes] = sol::bases<NI::TimeController, NI::Object>();
				setUserdataForNITimeController(usertypeDefinition);

				// Basic property binding.
				usertypeDefinition["axis"] = sol::property(&NI::LookAtController::getAxis, &NI::LookAtController::setAxis);
				usertypeDefinition["flip"] = sol::property(&NI::LookAtController::getFlip, &NI::LookAtController::setFlip);
				usertypeDefinition["lookAt"] = sol::property(&NI::LookAtController::getLookAt, &NI::LookAtController::setLookAt);
			}
		}
	}
}
