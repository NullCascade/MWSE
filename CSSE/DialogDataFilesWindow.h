#pragma once

#include "CSDefines.h"

namespace se::cs::dialog::data_files_window {
	struct DataFilesData {
		int result;
		RecordHandler* recordHandler;
		GameFile* selectedFile;
		void* listController;
		char selectedPath[MAX_PATH];
		int metadataChanged;
		GameFile* currentActiveFile;
		GameFile* originalActiveFile;
		BYTE mergeCompleted;
		BYTE canMerge;
		BYTE padding[2];
	};
	static_assert(sizeof(DataFilesData) == 0x124, "DataFilesData failed size validation");

	constexpr UINT DIALOG_ID = 162;

	// Default IDs.
	constexpr UINT CONTROL_ID_OK_BUTTON = 1;
	constexpr UINT CONTROL_ID_CANCEL_BUTTON = 2;
	constexpr UINT CONTROL_ID_SUMMARY_EDIT = 1024;
	constexpr UINT CONTROL_ID_CREATED_BY_EDIT = 1025;
	constexpr UINT CONTROL_ID_CREATED_ON_STATIC = 1026;
	constexpr UINT CONTROL_ID_LAST_MODIFIED_STATIC = 1027;
	constexpr UINT CONTROL_ID_DATA_FILES_LIST = 1056;
	constexpr UINT CONTROL_ID_MERGE_TO_MASTERS_BUTTON = 1119;
	constexpr UINT CONTROL_ID_SET_AS_ACTIVE_BUTTON = 1121;
	constexpr UINT CONTROL_ID_MASTERS_LIST = 1183;
	constexpr UINT CONTROL_ID_FILE_VERSION_STATIC = 1681;
	constexpr UINT CONTROL_ID_CURRENT_VERSION_STATIC = 1682;
	constexpr UINT CONTROL_ID_DETAILS_BUTTON = 1682;

	void installPatches();
}
