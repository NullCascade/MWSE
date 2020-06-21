#include "TES3WeaponLua.h"

#include "LuaManager.h"
#include "TES3ObjectLua.h"

#include "TES3Enchantment.h"
#include "TES3Script.h"
#include "TES3Weapon.h"

namespace mwse {
	namespace lua {
		void bindTES3Weapon() {
			// Get our lua state.
			auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
			sol::state& state = stateHandle.state;

			// Start our usertype. We must finish this with state.set_usertype.
			auto usertypeDefinition = state.new_usertype<TES3::Weapon>("tes3weapon");
			usertypeDefinition["new"] = sol::no_constructor;

			// Define inheritance structures. These must be defined in order from top to bottom. The complete chain must be defined.
			usertypeDefinition[sol::base_classes] = sol::bases<TES3::Item, TES3::PhysicalObject, TES3::Object, TES3::BaseObject>();
			setUserdataForTES3PhysicalObject(usertypeDefinition);

			// Basic property binding.
			usertypeDefinition["chopMax"] = &TES3::Weapon::chopMax;
			usertypeDefinition["chopMin"] = &TES3::Weapon::chopMin;
			usertypeDefinition["enchantCapacity"] = &TES3::Weapon::enchantCapacity;
			usertypeDefinition["enchantment"] = sol::property(&TES3::Weapon::getEnchantment, &TES3::Weapon::setEnchantment);
			usertypeDefinition["flags"] = &TES3::Weapon::materialFlags;
			usertypeDefinition["maxCondition"] = sol::property(&TES3::Weapon::getDurability, &TES3::Weapon::setDurability);
			usertypeDefinition["icon"] = sol::readonly_property(&TES3::Weapon::getIconPath);
			usertypeDefinition["mesh"] = sol::readonly_property(&TES3::Weapon::getModelPath);
			usertypeDefinition["name"] = sol::property(&TES3::Weapon::getName, &TES3::Weapon::setName);
			usertypeDefinition["reach"] = &TES3::Weapon::reach;
			usertypeDefinition["slashMax"] = &TES3::Weapon::slashMax;
			usertypeDefinition["slashMin"] = &TES3::Weapon::slashMin;
			usertypeDefinition["speed"] = &TES3::Weapon::speed;
			usertypeDefinition["thrustMax"] = &TES3::Weapon::thrustMax;
			usertypeDefinition["thrustMin"] = &TES3::Weapon::thrustMin;
			usertypeDefinition["type"] = sol::readonly_property(&TES3::Weapon::getType);
			usertypeDefinition["value"] = &TES3::Weapon::value;
			usertypeDefinition["weight"] = &TES3::Weapon::weight;

			// Access to other objects that need to be packaged.
			usertypeDefinition["script"] = sol::readonly_property(&TES3::Weapon::getScript);

			// Functions exposed as properties.
			usertypeDefinition["hasDurability"] = sol::readonly_property(&TES3::Weapon::hasDurability);
			usertypeDefinition["isOneHanded"] = sol::readonly_property(&TES3::Weapon::isOneHanded);
			usertypeDefinition["isTwoHanded"] = sol::readonly_property(&TES3::Weapon::isTwoHanded);
			usertypeDefinition["isMelee"] = sol::readonly_property(&TES3::Weapon::isMelee);
			usertypeDefinition["isRanged"] = sol::readonly_property(&TES3::Weapon::isRanged);
			usertypeDefinition["isAmmo"] = sol::readonly_property(&TES3::Weapon::isAmmo);
			usertypeDefinition["typeName"] = sol::readonly_property(&TES3::Weapon::getTypeName);

			// TODO: Deprecated. Remove before 2.1-stable.
			usertypeDefinition["health"] = sol::readonly_property(&TES3::Weapon::getDurability);
			usertypeDefinition["model"] = sol::readonly_property(&TES3::Weapon::getModelPath);
		}
	}
}
