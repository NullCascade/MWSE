#pragma once

#include "LuaObjectFilteredEvent.h"

namespace mwse {
	namespace lua {
		namespace event {
			class FilterBarterMenuEvent : public ObjectFilteredEvent {
			public:
				FilterBarterMenuEvent(TES3::UI::InventoryTile * tile, TES3::Item * item);
				sol::table createEventTable();

			protected:
				TES3::UI::InventoryTile * m_Tile;
				TES3::Item* m_Item;
			};
		}
	}
}
