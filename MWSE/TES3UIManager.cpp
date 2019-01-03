#include "MemoryUtil.h"
#include "TES3UIElement.h"
#include "TES3UIManager.h"
#include "TES3UIMenuController.h"
#include "TES3Util.h"
#include "TES3WorldController.h"

#include "Log.h"

#include <unordered_map>

namespace TES3 {
	namespace UI {
		const DWORD TES3_hook_dispatchMousewheelUp = 0x58F19B;
		const DWORD TES3_hook_dispatchMousewheelDown = 0x58F1CA;

		const auto TES3_uiHelpRoot = reinterpret_cast<Element* const*>(0x7D1C74);

		const auto TES3_ui_registerID = reinterpret_cast<UI_ID(__cdecl *)(const char *)>(0x58DF10);
		const auto TES3_ui_registerIDByString = reinterpret_cast<UI_ID(__cdecl *)(String, int)>(0x57B290);
		const auto TES3_ui_createChildElement = reinterpret_cast<Element* (__thiscall *)(Element*)>(0x582B50);
		const auto TES3_ui_createMenu = reinterpret_cast<Element* (__cdecl *)(UI_ID)>(0x595400);
		const auto TES3_ui_createTooltipMenu = reinterpret_cast<Element* (__cdecl *)(UI_ID)>(0x595A40);
		const auto TES3_ui_findMenu = reinterpret_cast<Element* (__cdecl*)(UI_ID)>(0x595370);
		const auto TES3_ui_findHelpLayerMenu = reinterpret_cast<Element* (__cdecl*)(UI_ID)>(0x595A10);
		const auto TES3_ui_getPaletteColour = reinterpret_cast<Vector3& (__cdecl*)(Vector3&, Property)>(0x57F610);
		const auto TES3_ui_onMenuUnfocus = reinterpret_cast<EventCallback>(0x58F790);
		const auto TES3_ui_ScrollbarArrow_onClick = reinterpret_cast<EventCallback>(0x647A60);
		const auto TES3_ui_requestMenuModeOn = reinterpret_cast<Boolean (__cdecl*)(UI_ID)>(0x595230);
		const auto TES3_ui_requestMenuModeOff = reinterpret_cast<Boolean (__cdecl*)(UI_ID)>(0x595270);
		const auto TES3_ui_getServiceActor = reinterpret_cast<MobileActor* (__cdecl*)()>(0x5BFEA0);
		const auto TES3_ui_updateDialogDisposition = reinterpret_cast<void (__cdecl*)()>(0x5C0780);

		std::unordered_map<UI_ID, std::string> uiIdNameMap;

		//
		// UI framework functions
		//

		UI_ID registerID(const char *name) {
			return TES3_ui_registerID(name);
		}

		UI_ID registerIDWithString(String name, int unknown) {
			auto result = TES3_ui_registerIDByString(name, unknown);

			auto mapHit = uiIdNameMap.find(result);
			if (mapHit == uiIdNameMap.end()) {
				mwse::log::getLog() << "Property mapped: " << result << " -> " << name.cString << std::endl;
				uiIdNameMap[result] = name.cString;
			}

			return result;
		}

		Property registerProperty(const char *name) {
			return static_cast<Property>(TES3_ui_registerID(name));
		}

		Element* createMenu(UI_ID id) {
			return TES3_ui_createMenu(id);
		}

		Element* createHelpLayerMenu(UI_ID id) {
			// Simple replacement implementation. The Morrowind implementation is convoluted.
			Element* menu = TES3_ui_createChildElement(*TES3_uiHelpRoot);
			menu->createFixedFrame(id, 1);
			return menu;
		}

		Element* createTooltipMenu(UI_ID id) {
			return TES3_ui_createTooltipMenu(id);
		}

		Element* findMenu(UI_ID id) {
			return TES3_ui_findMenu(id);
		}

		Element* findHelpLayerMenu(UI_ID id) {
			return TES3_ui_findHelpLayerMenu(id);
		}

		Boolean enterMenuMode(UI_ID id) {
			return TES3_ui_requestMenuModeOn(id);
		}

		Boolean leaveMenuMode() {
			return TES3_ui_requestMenuModeOff(0);
		}

		void acquireTextInput(Element* element) {
			mwse::tes3::getWorldController()->menuController->menuInputController->acquireTextInput(element);
		}

		void preventInventoryMenuToggle(Element* menu) {
			menu->setProperty(Property::event_unfocus, TES3_ui_onMenuUnfocus);
		}

		Vector3 getPaletteColour(Property prop) {
			Vector3 colour;
			return TES3_ui_getPaletteColour(colour, prop);
		}

		//
		// Added functionality
		//

		Boolean __cdecl onScrollPaneMousewheel(Element* owningWidget, Property eventID, int data0, int data1, Element* source) {
			static auto propScrollPane = registerProperty("PartScrollPane");
			static auto propScrollPaneHor = registerProperty("PartScrollPaneHor");
			static auto propScrollPaneVert = registerProperty("PartScrollPaneVert");
			static auto propScrollbarHor = registerID("PartScrollPane_hor_scrollbar");
			static auto propScrollbarVert = registerID("PartScrollPane_vert_scrollbar");
			static auto propScrollLeftArrow = registerID("PartScrollBar_left_arrow");
			static auto propScrollRightArrow = registerID("PartScrollBar_right_arrow");

			Element* scrollbar = nullptr;
			Element* pane;
			for (pane = source; pane; pane = pane->parent) {
				Property part = pane->getProperty(PropertyType::Property, Property::is_part).propertyValue;
				if (part == propScrollPaneVert) {
					scrollbar = pane->findChild(propScrollbarVert);
					break;
				}
				else if (part == propScrollPane || part == propScrollPaneHor) {
					scrollbar = pane->findChild(propScrollbarHor);
					break;
				}
			}

			if (scrollbar) {
				// Forward event to left or right arrow depending on scroll direction in data0
				UI_ID arrowID = (data0 < 0) ? propScrollRightArrow : propScrollLeftArrow;
				Element* arrow = scrollbar->findChild(arrowID);
				return TES3_ui_ScrollbarArrow_onClick(scrollbar, eventID, data0, data1, arrow);
			}
			return 1;
		}

		MobileActor* getServiceActor() {
			return TES3_ui_getServiceActor();
		}

		void updateDialogDisposition() {
			TES3_ui_updateDialogDisposition();
		}

		const char* getInventorySelectType() {
			const char* callbackType = "unknown";
			auto callbackAddress = *reinterpret_cast<DWORD*>(0x7D3CA0);
			switch (callbackAddress) {
			case 0x59A1F0: callbackType = "alembic"; break;
			case 0x59A160: callbackType = "calcinator"; break;
			case 0x5C6B40: callbackType = "enchantedItem"; break;
			case 0x59A220: callbackType = "ingredient"; break;
			case 0x59A190: callbackType = "mortar"; break;
			case 0x608A90: callbackType = "quick"; break;
			case 0x59A1C0: callbackType = "retort"; break;
			case 0x5C6B00: callbackType = "soulGemFilled"; break;
			}
			return callbackType;
		}

		//
		// UI framework improvement hooks
		//

		__declspec(naked) void patchDispatchMousewheelUp() {
			__asm {
				mov eax, [esi + 4]
				mov edx, [esi + 8]
				push ecx
				push edx
				push eax
				push 0xFFFF8036
				jmp $ + 0x37C
			}
		}
		const size_t patchDispatchMousewheelUp_size = 0x14;

		__declspec(naked) void patchDispatchMousewheelDown() {
			__asm {
				mov eax, [esi + 4]
				mov edx, [esi + 8]
				push ecx
				push edx
				push eax
				push 0xFFFF8037
				jmp $ + 0x34D
			}
		}
		const size_t patchDispatchMousewheelDown_size = 0x14;

		const auto TES3_ConsoleLogResult = reinterpret_cast<void(__cdecl*)(const char*, bool)>(0x5B2C20);
		void logToConsole(const char* text, bool isCommand) {
			TES3_ConsoleLogResult(text, isCommand);
		}

		const auto TES3_ShowBookMenu = reinterpret_cast<void(__cdecl*)(const char*)>(0x5AC2A0);
		void showBookMenu(const char* text) {
			TES3_ShowBookMenu(text);
		}

		const auto TES3_ShowScrollMenu = reinterpret_cast<void(__cdecl*)(const char*)>(0x6138A0);
		void showScrollMenu(const char* text) {
			TES3_ShowScrollMenu(text);
		}

		const auto TES3_UpdateInventoryTiles = reinterpret_cast<void(__cdecl*)()>(0x5CC910);
		void updateInventoryMenuTiles() {
			TES3_UpdateInventoryTiles();
		}

		const auto TES3_UpdateContentsMenuTiles = reinterpret_cast<void(__cdecl*)()>(0x5B67E0);
		void updateContentsMenuTiles() {
			TES3_UpdateContentsMenuTiles();
		}

		const auto TES3_UpdateBarterMenuTiles = reinterpret_cast<void(__cdecl*)()>(0x5A5620);
		void updateBarterMenuTiles() {
			TES3_UpdateBarterMenuTiles();
		}

		const auto TES3_UpdateInventorySelectTiles = reinterpret_cast<int(__cdecl*)()>(0x5D3E70);
		int updateSelectInventoryTiles() {
			return TES3_UpdateInventorySelectTiles();
		}

		const char* getNameForUIID(UI_ID id) {
			auto result = uiIdNameMap.find(id);
			if (result != uiIdNameMap.end()) {
				return result->second.c_str();
			}
			return nullptr;
		}

		void hook() {
			// Patch mousewheel event dispatch to not redirect to the top-level element,
			// allowing mousewheel to apply to more than the first scrollpane in a menu
			mwse::writePatchCodeUnprotected(TES3_hook_dispatchMousewheelUp, (BYTE*)&patchDispatchMousewheelUp, patchDispatchMousewheelUp_size);
			mwse::writePatchCodeUnprotected(TES3_hook_dispatchMousewheelDown, (BYTE*)&patchDispatchMousewheelDown, patchDispatchMousewheelDown_size);

			// Patch UI layout engine to reflow wrapped text content on size changes
			auto patch = &Element::patchUpdateLayout_propagateFlow;
			mwse::genCallEnforced(0x585E1E, 0x584850, *reinterpret_cast<DWORD*>(&patch));
			mwse::genCallEnforced(0x5863AE, 0x584850, *reinterpret_cast<DWORD*>(&patch));

			// Patch UI system so that newly created IDs can be looked up.
			mwse::genCallEnforced(0x58DF3D, 0x57B290, reinterpret_cast<DWORD>(registerIDWithString));
			mwse::genCallEnforced(0x593F36, 0x57B290, reinterpret_cast<DWORD>(registerIDWithString));
			mwse::genCallEnforced(0x5DDFCD, 0x57B290, reinterpret_cast<DWORD>(registerIDWithString));
			mwse::genCallEnforced(0x5DE154, 0x57B290, reinterpret_cast<DWORD>(registerIDWithString));
			mwse::genCallEnforced(0x5DE1D9, 0x57B290, reinterpret_cast<DWORD>(registerIDWithString));
			mwse::genCallEnforced(0x5DE2DB, 0x57B290, reinterpret_cast<DWORD>(registerIDWithString));
			mwse::genCallEnforced(0x5E8881, 0x57B290, reinterpret_cast<DWORD>(registerIDWithString));
			mwse::genCallEnforced(0x611FA0, 0x57B290, reinterpret_cast<DWORD>(registerIDWithString));
			mwse::genCallEnforced(0x61242E, 0x57B290, reinterpret_cast<DWORD>(registerIDWithString));
			mwse::genCallEnforced(0x61270A, 0x57B290, reinterpret_cast<DWORD>(registerIDWithString));
			mwse::genCallEnforced(0x637D55, 0x57B290, reinterpret_cast<DWORD>(registerIDWithString));
			mwse::genCallEnforced(0x637DAC, 0x57B290, reinterpret_cast<DWORD>(registerIDWithString));
			mwse::genCallEnforced(0x638DE1, 0x57B290, reinterpret_cast<DWORD>(registerIDWithString));
			mwse::genCallEnforced(0x63B1E1, 0x57B290, reinterpret_cast<DWORD>(registerIDWithString));
			mwse::genCallEnforced(0x63E4E1, 0x57B290, reinterpret_cast<DWORD>(registerIDWithString));
			mwse::genCallEnforced(0x6406B1, 0x57B290, reinterpret_cast<DWORD>(registerIDWithString));
			mwse::genCallEnforced(0x642501, 0x57B290, reinterpret_cast<DWORD>(registerIDWithString));
			mwse::genCallEnforced(0x643051, 0x57B290, reinterpret_cast<DWORD>(registerIDWithString));
			mwse::genCallEnforced(0x645931, 0x57B290, reinterpret_cast<DWORD>(registerIDWithString));
			mwse::genCallEnforced(0x6484C1, 0x57B290, reinterpret_cast<DWORD>(registerIDWithString));
			mwse::genCallEnforced(0x648501, 0x57B290, reinterpret_cast<DWORD>(registerIDWithString));
			mwse::genCallEnforced(0x648541, 0x57B290, reinterpret_cast<DWORD>(registerIDWithString));
			mwse::genCallEnforced(0x64A841, 0x57B290, reinterpret_cast<DWORD>(registerIDWithString));
			mwse::genCallEnforced(0x64BBA1, 0x57B290, reinterpret_cast<DWORD>(registerIDWithString));
			mwse::genCallEnforced(0x64E14B, 0x57B290, reinterpret_cast<DWORD>(registerIDWithString));
			mwse::genCallEnforced(0x64E19E, 0x57B290, reinterpret_cast<DWORD>(registerIDWithString));
			mwse::genCallEnforced(0x64E28C, 0x57B290, reinterpret_cast<DWORD>(registerIDWithString));
			mwse::genCallEnforced(0x64E2DF, 0x57B290, reinterpret_cast<DWORD>(registerIDWithString));
			mwse::genCallEnforced(0x64E37D, 0x57B290, reinterpret_cast<DWORD>(registerIDWithString));
			mwse::genCallEnforced(0x64E3F5, 0x57B290, reinterpret_cast<DWORD>(registerIDWithString));
			mwse::genCallEnforced(0x64E43D, 0x57B290, reinterpret_cast<DWORD>(registerIDWithString));
			mwse::genCallEnforced(0x64E47C, 0x57B290, reinterpret_cast<DWORD>(registerIDWithString));
		}

	}
}
