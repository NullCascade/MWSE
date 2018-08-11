/************************************************************************
	
	main.cpp - Copyright (c) 2008 The MWSE Project
	https://github.com/MWSE/MWSE/

	This program is free software; you can redistribute it and/or
	modify it under the terms of the GNU General Public License
	as published by the Free Software Foundation; either version 2
	of the License, or (at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program; if not, write to the Free Software
	Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

**************************************************************************/

#include <windows.h>
#include <dbghelp.h>
#include <filesystem>

#include "mwAdapter.h"
#include "Log.h"
#include "MgeTes3Machine.h"

#include "TES3Util.h"
#include "CodePatchUtil.h"
#include "PatchUtil.h"
#include "MWSEDefs.h"

#include "LuaManager.h"
#include "TES3Game.h"

TES3MACHINE* mge_virtual_machine = NULL;
void* external_malloc = NULL;
void* external_free = NULL;
void* external_realloc = NULL;

static BOOL CALLBACK EnumSymbolsCallback(PSYMBOL_INFO, ULONG, PVOID);

struct VersionStruct {
	BYTE major;
	BYTE minor;
	BYTE patch;
	BYTE build;
};

VersionStruct GetMGEVersion() {
	DWORD dwSize = GetFileVersionInfoSize("MGEXEgui.exe", NULL);
	if (dwSize == 0) {
		return VersionStruct{};
	}

	BYTE * pbVersionInfo = new BYTE[dwSize];
	if (!GetFileVersionInfo("MGEXEgui.exe", 0, dwSize, pbVersionInfo)) {
		delete[] pbVersionInfo;
		return VersionStruct{};
	}

	VS_FIXEDFILEINFO * pFileInfo = NULL;
	UINT puLenFileInfo = 0;
	if (!VerQueryValue(pbVersionInfo, TEXT("\\"), (LPVOID*)&pFileInfo, &puLenFileInfo)) {
		delete[] pbVersionInfo;
		return VersionStruct{};
	}

	VersionStruct version;
	version.major = HIWORD(pFileInfo->dwProductVersionMS);
	version.minor = LOWORD(pFileInfo->dwProductVersionMS);
	version.patch = LOWORD(pFileInfo->dwProductVersionLS >> 16);
	version.build = HIWORD(pFileInfo->dwProductVersionLS >> 16);
	delete[] pbVersionInfo;

	return version;
}

bool __fastcall OnGameStructInitialized(TES3::Game * game) {
	// Setup our lua interface before initializing.
	mwse::lua::LuaManager::getInstance().hook();

	return game->initialize();
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpReserved) {
	HANDLE process = NULL;
	switch (fdwReason) {
	case DLL_PROCESS_ATTACH:
	{
		// Initialize once for each new process. Return FALSE to fail DLL load.

		// Initialize log file.
		mwse::log::OpenLog("MWSELog.txt");
		mwse::log::getLog() << "Morrowind Script Extender v" << MWSE_VERSION_MAJOR << "." << MWSE_VERSION_MINOR << "." << MWSE_VERSION_PATCH << " (built " << __DATE__ << ") hooked." << std::endl;

		// Before we do anything else, ensure that we can make minidumps.
		if (!mwse::patch::installMiniDumpHook()) {
			mwse::log::getLog() << "Warning: Unable to hook minidump! Crash dumps will be unavailable." << std::endl;
		}

		// Make sure we have the right version of MGE XE installed.
		VersionStruct mgeVersion = GetMGEVersion();
		if (mgeVersion.major == 0 && mgeVersion.minor == 0) {
			mwse::log::getLog() << "Error: Could not determine MGE XE version." << std::endl;
			MessageBox(NULL, "MGE XE does not seem to be installed. Please install MGE XE v0.10.0.0 or later.", "MGE XE Check Failed", MB_ICONERROR | MB_OK);
			exit(0);
		}
		else if (mgeVersion.major == 0 && mgeVersion.minor < 10) {
			mwse::log::getLog() << "Invalid MGE XE version: " << (int)mgeVersion.major << "." << (int)mgeVersion.minor << "." << (int)mgeVersion.patch << "." << (int)mgeVersion.build << std::endl;

			std::stringstream ss;
			ss << "Invalid MGE XE version found. Minimum version is 0.10.0.0.\nFound version: ";
			ss << "Found MGE XE v" << (int)mgeVersion.major << "." << (int)mgeVersion.minor << "." << (int)mgeVersion.patch << "." << (int)mgeVersion.build;
			MessageBox(NULL, ss.str().c_str(), "MGE XE Check Failed", MB_ICONERROR | MB_OK);
			exit(0);
		}
		else {
			mwse::log::getLog() << "Found MGE XE. Version: " << (int)mgeVersion.major << "." << (int)mgeVersion.minor << "." << (int)mgeVersion.patch << "." << (int)mgeVersion.build << std::endl;
		}

		// Look to see if an update to the MWSE Updater was downloaded. If so, swap the exes.
		if (std::experimental::filesystem::exists("MWSE-Update.tmp")) {
			if (std::experimental::filesystem::exists("MWSE-Update.exe")) {
				std::experimental::filesystem::remove("MWSE-Update.exe");
			}
			std::experimental::filesystem::rename("MWSE-Update.tmp", "MWSE-Update.exe");
		}

		// Initialize our main mwscript hook.
		mwse::mwAdapter::Hook();

		// Create MGE VM interface.
		mge_virtual_machine = new TES3MACHINE();

		// Find the addresses of malloc(), realloc(), free() that MW uses, so that we can interact with its heap.
		process = GetCurrentProcess();
		SymInitialize(process, NULL, TRUE);
		SymEnumSymbols(process, 0, "msvcrt!*", EnumSymbolsCallback, NULL);
		SymCleanup(process);

		// Ensure that we got malloc.
		if (external_malloc == NULL) {
			mwse::log::getLog() << "Error: unable to find malloc()" << std::endl;
			exit(1);
		}
		else {
			mwse::tes3::_malloc = reinterpret_cast<mwse::tes3::ExternalMalloc>(external_malloc);
		}

		// Ensure that we got free.
		if (external_free == NULL) {
			mwse::log::getLog() << "Error: unable to find free()" << std::endl;
			exit(1);
		}
		else {
			mwse::tes3::_free = reinterpret_cast<mwse::tes3::ExternalFree>(external_free);
		}

		// Ensure that we got realloc.
		if (external_realloc == NULL) {
			mwse::log::getLog() << "Error: unable to find realloc()" << std::endl;
			exit(1);
		}
		else {
			mwse::tes3::_realloc = reinterpret_cast<mwse::tes3::ExternalRealloc>(external_realloc);
		}

		// Install necessary patches.
		mwse::patch::installPatches();

		// Parse and load the features installed by the Morrowind Code Patch.
		if (!mwse::mcp::loadFeatureList()) {
			mwse::log::getLog() << "Failed to detect Morrowind Code Patch installed features. MCP may not be installed, or the mcpatch\\installed file may have been deleted. Mods will be unable to detect MCP feature support." << std::endl;
		}

		// Delay our lua hook until later, to ensure that Mod Organizer's VFS is hooked up.
		mwse::genCallEnforced(0x417195, 0x417880, reinterpret_cast<DWORD>(OnGameStructInitialized));
	}
	break;
	case DLL_THREAD_ATTACH:
		// Do thread-specific initialization.
		break;
	case DLL_THREAD_DETACH:
		// Do thread-specific cleanup.
		break;
	case DLL_PROCESS_DETACH:
		// Unhook Lua interface.
		mwse::lua::LuaManager::getInstance().cleanup();
		break;
	}

	return TRUE; // Successful DLL_PROCESS_ATTACH.
}

// MGE XE exports
extern "C"
{
	__declspec(dllexport) TES3MACHINE* MWSEGetVM();
	__declspec(dllexport) bool MWSEAddInstruction(OPCODE op, INSTRUCTION *ins);
}

TES3MACHINE* MWSEGetVM()
{
	return mge_virtual_machine;
}

bool MWSEAddInstruction(OPCODE op, INSTRUCTION *ins)
{
	return mge_virtual_machine->AddInstruction(op, ins);
}

static BOOL CALLBACK EnumSymbolsCallback(PSYMBOL_INFO symbol_info,
	ULONG /*symbol_size*/,
	PVOID /*user_context*/)
{
	if (strcmp(symbol_info->Name, "malloc") == 0) {
		external_malloc = reinterpret_cast<void*>(symbol_info->Address);
	}
	else if (strcmp(symbol_info->Name, "free") == 0) {
		external_free = reinterpret_cast<void*>(symbol_info->Address);
	}
	else if (strcmp(symbol_info->Name, "realloc") == 0) {
		external_realloc = reinterpret_cast<void*>(symbol_info->Address);
	}
	return TRUE;
}
