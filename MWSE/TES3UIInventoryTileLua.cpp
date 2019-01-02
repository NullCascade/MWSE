#include "TES3UIInventoryTileLua.h"

#include "LuaManager.h"
#include "LuaUtil.h"

#include "TES3UIElement.h"
#include "TES3UIInventoryTile.h"

namespace mwse {
	namespace lua {
		void bindTES3UIInventoryTile() {
			// Get our lua state.
			sol::state& state = LuaManager::getInstance().getState();

			// Start our usertype. We must finish this with state.set_usertype.
			auto usertypeDefinition = state.create_simple_usertype<TES3::UI::InventoryTile>();
			usertypeDefinition.set("new", sol::no_constructor);

			// Functions exposed as properties.
			usertypeDefinition.set("count", sol::readonly_property(&TES3::UI::InventoryTile::count));
			usertypeDefinition.set("element", sol::readonly_property(&TES3::UI::InventoryTile::element));
			usertypeDefinition.set("isBoundItem", sol::readonly_property(&TES3::UI::InventoryTile::isBoundItem));
			usertypeDefinition.set("itemData", sol::readonly_property(&TES3::UI::InventoryTile::itemData));
			usertypeDefinition.set("type", sol::readonly_property(&TES3::UI::InventoryTile::tileType));

			// Access to other objects that need to be packaged.
			usertypeDefinition.set("item", sol::readonly_property([](TES3::UI::InventoryTile& self) { return makeLuaObject(self.item); }));

			// Finish up our usertype.
			state.set_usertype("tes3inventoryTile", usertypeDefinition);
		}
	}
}