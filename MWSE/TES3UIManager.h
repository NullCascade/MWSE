#pragma once

#include "TES3UIDefines.h"
#include "TES3Vectors.h"

#include <string>

namespace TES3 {
	namespace UI {
		__declspec(dllexport) UI_ID registerID(const char* name);
		__declspec(dllexport) UI_ID __cdecl registerIDWithString(String name, int unknown = 0);
		__declspec(dllexport) Property registerProperty(const char* name);
		__declspec(dllexport) Element* createMenu(UI_ID id);
		__declspec(dllexport) Element* createHelpLayerMenu(UI_ID id);
		__declspec(dllexport) Element* createTooltipMenu(UI_ID id);
		__declspec(dllexport) Element* findMenu(UI_ID id);
		__declspec(dllexport) Element* findHelpLayerMenu(UI_ID id);
		__declspec(dllexport) Boolean enterMenuMode(UI_ID id);
		__declspec(dllexport) Boolean leaveMenuMode();
		__declspec(dllexport) void acquireTextInput(Element* element);
		__declspec(dllexport) void preventInventoryMenuToggle(Element* menu);
		__declspec(dllexport) Vector3 getPaletteColour(Property prop);

		Boolean __cdecl onScrollPaneMousewheel(Element*, Property, int, int, Element*);

		__declspec(dllexport) MobileActor* getServiceActor();
		__declspec(dllexport) void updateDialogDisposition();

		__declspec(dllexport) const char* getInventorySelectType();

		__declspec(dllexport) void logToConsole(const char* text, bool isCommand = false);

		__declspec(dllexport) void showBookMenu(const char* text);
		__declspec(dllexport) void showScrollMenu(const char* text);

		__declspec(dllexport) void updateInventoryMenuTiles();
		__declspec(dllexport) void updateContentsMenuTiles();
		__declspec(dllexport) void updateBarterMenuTiles();
		__declspec(dllexport) int updateSelectInventoryTiles();

		__declspec(dllexport) const char* getNameForUIID(UI_ID);

		void hook();
	}
}
