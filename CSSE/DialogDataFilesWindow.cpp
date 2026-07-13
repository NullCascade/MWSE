#include "DialogDataFilesWindow.h"

#include "CSGameFile.h"
#include "CSRecordHandler.h"
#include "DialogProcContext.h"
#include "MemoryUtil.h"

namespace se::cs::dialog::data_files_window {
	void PatchDialogProc_BeforeCommand(DialogProcContext& context) {
		if (context.getCommandControlIdentifier() != CONTROL_ID_SET_AS_ACTIVE_BUTTON) {
			return;
		}

		const auto data = context.getUserData<DataFilesData>();
		if (data == nullptr || data->selectedFile == nullptr || !data->selectedFile->isOpenMWAddonAlias()) {
			return;
		}

		std::string sourceName;
		data->selectedFile->getOpenMWAddonSourceName(sourceName);
		const auto message = fmt::format(
			"'{}' is an OpenMW addon and is loaded read-only. Writing back to it could discard OpenMW-specific data. Set an ESP as the active file, or save your edits to a new ESP.",
			sourceName
		);
		MessageBoxA(context.getWindowHandle(), message.c_str(), "OpenMW Addon (Read-Only)", MB_OK | MB_ICONINFORMATION);
		context.setResult(TRUE);
	}

	void PatchDialogProc_AfterGetDisplayInfo(DialogProcContext& context) {
		const auto displayInfo = context.getNotificationListViewDisplayInfo();
		if (displayInfo == nullptr || displayInfo->item.pszText == nullptr) {
			return;
		}

		const auto data = context.getUserData<DataFilesData>();
		if (data == nullptr || data->recordHandler == nullptr || displayInfo->item.iItem < 0) {
			return;
		}
		const auto gameFile = data->recordHandler->getAvailableGameFileByIndex(displayInfo->item.iItem);
		static thread_local std::string sourceName;
		if (gameFile && gameFile->getOpenMWAddonSourceName(sourceName)) {
			displayInfo->item.pszText = sourceName.data();
		}
	}

	void PatchDialogProc_AfterItemChanged(DialogProcContext& context) {
		const auto data = context.getUserData<DataFilesData>();
		if (data == nullptr || data->selectedFile == nullptr || !data->selectedFile->isOpenMWAddonAlias()) {
			return;
		}

		const auto hWnd = context.getWindowHandle();
		EnableWindow(GetDlgItem(hWnd, CONTROL_ID_SET_AS_ACTIVE_BUTTON), FALSE);
		EnableWindow(GetDlgItem(hWnd, CONTROL_ID_MERGE_TO_MASTERS_BUTTON), FALSE);
		EnableWindow(GetDlgItem(hWnd, CONTROL_ID_CREATED_BY_EDIT), FALSE);
		EnableWindow(GetDlgItem(hWnd, CONTROL_ID_SUMMARY_EDIT), FALSE);
	}

	LRESULT CALLBACK PatchDialogProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
		DialogProcContext context(hWnd, msg, wParam, lParam, 0x415A40);

		if (msg == WM_COMMAND) {
			PatchDialogProc_BeforeCommand(context);
		}

		if (context.hasResult()) {
			return context.getResult();
		}
		context.callOriginalFunction();

		if (msg == WM_NOTIFY) {
			const auto notification = context.getNotificationData();
			if (notification && notification->idFrom == CONTROL_ID_DATA_FILES_LIST) {
				switch (notification->code) {
				case LVN_GETDISPINFOA:
					PatchDialogProc_AfterGetDisplayInfo(context);
					break;
				case LVN_ITEMCHANGED:
					PatchDialogProc_AfterItemChanged(context);
					break;
				}
			}
		}

		return context.getResult();
	}

	void installPatches() {
		using memory::genJumpEnforced;

		// Patch: Present prepared OpenMW addon aliases by source identity and keep them read-only.
		genJumpEnforced(0x403891, 0x415A40, reinterpret_cast<DWORD>(PatchDialogProc));
	}
}
