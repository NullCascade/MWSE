#include "TES3LockpickLua.h"

#include "LuaManager.h"

#include "TES3Lockpick.h"
#include "TES3Script.h"

namespace mwse {
	namespace lua {
		void bindTES3Lockpick() {
			LuaManager::getInstance().getState().new_usertype<TES3::Lockpick>("TES3Lockpick",
				// Disable construction of this type.
				"new", sol::no_constructor,

				//
				// Properties.
				//

				"objectType", &TES3::Lockpick::objectType,

				"id", sol::readonly_property(&TES3::Lockpick::getObjectID),
				"name", sol::property(&TES3::Lockpick::getName, &TES3::Lockpick::setName),

				"icon", sol::readonly_property(&TES3::Lockpick::getIconPath),
				"model", sol::readonly_property(&TES3::Lockpick::getModelPath),

				"value", sol::readonly_property(&TES3::Lockpick::getValue),
				"weight", sol::readonly_property(&TES3::Lockpick::getWeight),
				"quality", &TES3::Lockpick::quality,
				"condition", &TES3::Lockpick::maxCondition,

				"script", sol::readonly_property(&TES3::Lockpick::getScript)

				);
		}
	}
}