#include "TES3RaceLua.h"

#include "LuaManager.h"
#include "TES3ObjectLua.h"

#include "TES3BodyPart.h"
#include "TES3Race.h"

namespace mwse {
	namespace lua {
		void bindTES3Race() {
			// Get our lua state.
			auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
			sol::state& state = stateHandle.state;

			// Binding for TES3::Race::SkillBonus.
			{
				// Start our usertype. We must finish this with state.set_usertype.
				auto usertypeDefinition = state.new_usertype<TES3::Race::SkillBonus>("tes3raceSkillBonus");
				usertypeDefinition["new"] = sol::no_constructor;

				// Basic property binding.
				usertypeDefinition["skill"] = &TES3::Race::SkillBonus::skill;
				usertypeDefinition["bonus"] = &TES3::Race::SkillBonus::bonus;
			}

			// Binding for TES3::Race::BaseAttribute.
			{
				// Start our usertype. We must finish this with state.set_usertype.
				auto usertypeDefinition = state.new_usertype<TES3::Race::BaseAttribute>("tes3raceBaseAttribute");
				usertypeDefinition["new"] = sol::no_constructor;

				// Basic property binding.
				usertypeDefinition["male"] = &TES3::Race::BaseAttribute::male;
				usertypeDefinition["female"] = &TES3::Race::BaseAttribute::female;
			}

			// Binding for TES3::Race::HeightWeight.
			{
				// Start our usertype. We must finish this with state.set_usertype.
				auto usertypeDefinition = state.new_usertype<TES3::Race::HeightWeight>("tes3raceHeightWeight");
				usertypeDefinition["new"] = sol::no_constructor;

				// Basic property binding.
				usertypeDefinition["male"] = &TES3::Race::HeightWeight::male;
				usertypeDefinition["female"] = &TES3::Race::HeightWeight::female;
			}

			// Binding for TES3::Race::BodyParts.
			{
				// Start our usertype. We must finish this with state.set_usertype.
				auto usertypeDefinition = state.new_usertype<TES3::Race::BodyParts>("tes3raceBodyParts");
				usertypeDefinition["new"] = sol::no_constructor;

				// Basic property binding.
				usertypeDefinition["head1"] = &TES3::Race::BodyParts::head;
				usertypeDefinition["hair"] = &TES3::Race::BodyParts::hair;
				usertypeDefinition["neck"] = &TES3::Race::BodyParts::neck;
				usertypeDefinition["chest"] = &TES3::Race::BodyParts::chest;
				usertypeDefinition["groin"] = &TES3::Race::BodyParts::groin;
				usertypeDefinition["hands"] = &TES3::Race::BodyParts::hands;
				usertypeDefinition["wrist"] = &TES3::Race::BodyParts::wrist;
				usertypeDefinition["forearm"] = &TES3::Race::BodyParts::forearm;
				usertypeDefinition["upperArm"] = &TES3::Race::BodyParts::upperArm;
				usertypeDefinition["foot"] = &TES3::Race::BodyParts::foot;
				usertypeDefinition["ankle"] = &TES3::Race::BodyParts::ankle;
				usertypeDefinition["knee"] = &TES3::Race::BodyParts::knee;
				usertypeDefinition["upperLeg"] = &TES3::Race::BodyParts::upperLeg;
				usertypeDefinition["clavicle"] = &TES3::Race::BodyParts::clavicle;
				usertypeDefinition["tail"] = &TES3::Race::BodyParts::tail;
				usertypeDefinition["head2"] = &TES3::Race::BodyParts::vampireHead;
			}

			// Binding for TES3::Race.
			{
				// Start our usertype. We must finish this with state.set_usertype.
				auto usertypeDefinition = state.new_usertype<TES3::Race>("tes3race");
				usertypeDefinition["new"] = sol::no_constructor;

				// Define inheritance structures. These must be defined in order from top to bottom. The complete chain must be defined.
				usertypeDefinition[sol::base_classes] = sol::bases<TES3::BaseObject>();
				setUserdataForTES3BaseObject(usertypeDefinition);

				// Basic property binding.
				usertypeDefinition["femaleBody"] = sol::readonly_property(&TES3::Race::femaleBody);
				usertypeDefinition["flags"] = &TES3::Race::flags;
				usertypeDefinition["height"] = sol::readonly_property(&TES3::Race::height);
				usertypeDefinition["maleBody"] = sol::readonly_property(&TES3::Race::maleBody);
				usertypeDefinition["weight"] = sol::readonly_property(&TES3::Race::weight);

				// Indirect bindings to unions and arrays.
				usertypeDefinition["baseAttributes"] = sol::readonly_property([](TES3::Race& self) { return std::ref(self.baseAttributes); });
				usertypeDefinition["skillBonuses"] = sol::readonly_property([](TES3::Race& self) { return std::ref(self.skillBonuses); });

				// Functions exposed as properties.
				usertypeDefinition["name"] = sol::readonly_property([](TES3::Race& self) { return self.name; });
				// Description may need to be loaded from disk, handle it specially.
				usertypeDefinition["description"] = sol::readonly_property(
					[](TES3::Race& self) -> sol::object
				{
					auto& luaManager = mwse::lua::LuaManager::getInstance();
					auto stateHandle = luaManager.getThreadSafeStateHandle();
					sol::state& state = stateHandle.state;

					// If the description is already loaded, just return it.
					if (self.description) {
						return sol::make_object(state, self.description);
					}

					// Otherwise we need to load it from disk, then free it.
					else {
						char* description = self.loadDescription();
						if (description) {
							// We loaded successfully, package, free, then return.
							sol::object value = sol::make_object(state, description);
							self.freeDescription();
							return value;
						}
					}

					return sol::nil;
				}
				);


			}
		}
	}
}
