#pragma once

#include "OpenMWLuaBridge.h"

namespace mwse::openmw {

	class HostController {
	public:
		static HostController& getInstance();

		void initialize();
		void shutdown();
		void update(double deltaSeconds, double simulationTimeSeconds, bool paused);
		bool reload();
		std::string getReport() const;
		LifecycleState getLifecycleState() const;
		bool isLoaded() const;

	private:
		HostController() = default;
		static void __cdecl receiveLog(void* userData, const LogMessage* message);
		static Status __cdecl getGameSetting(void* userData, StringView name, BridgeValue* value);
		static std::uint32_t __cdecl getContentFileCount(void* userData);
		static Status __cdecl getContentFile(void* userData, std::uint32_t index, StringView* contentFile);
		static Status __cdecl getPlayerObject(void* userData, ObjectSnapshot* snapshot);
		static Status __cdecl validateHandle(void* userData, OpaqueHandle handle, HandleType expectedType);
		static Status __cdecl getCell(void* userData, OpaqueHandle handle, CellSnapshot* snapshot);
		static Status __cdecl getStat(void* userData, OpaqueHandle actor, StatKind kind,
			std::uint32_t index, StatSnapshot* snapshot);
		static Status __cdecl setStat(void* userData, OpaqueHandle actor, StatKind kind,
			std::uint32_t index, StatField field, double value);
		static Status __cdecl getRecordCount(void* userData, RecordType type, std::uint32_t* count);
		static Status __cdecl getRecord(void* userData, RecordType type, std::uint32_t index,
			StringView id, RecordSnapshot* snapshot);
		static Status __cdecl getActorSpellCount(void* userData, OpaqueHandle actor, std::uint32_t* count);
		static Status __cdecl getActorSpell(void* userData, OpaqueHandle actor, std::uint32_t index,
			RecordSnapshot* snapshot);
		static Status __cdecl setActorSpell(void* userData, OpaqueHandle actor, StringView id, std::uint32_t add);
		static Status __cdecl getActiveSpellCount(void* userData, OpaqueHandle actor, std::uint32_t* count);
		static Status __cdecl getActiveSpell(void* userData, OpaqueHandle actor, std::uint32_t index,
			ActiveSpellSnapshot* snapshot);
		static std::string copyLogString(StringView value);
		static std::string getEnvironment(const char* name);
		static bool environmentEnabled(const char* name);
		bool isGameDataReady() const;
		Status startRuntime();
		void resetHandles();
		void restoreHarnessMutations();
		OpaqueHandle playerOpaqueHandle() const;
		OpaqueHandle cellOpaqueHandle(void* cell);
		Status validateOpaqueHandle(OpaqueHandle handle, HandleType expectedType, void** nativeValue) const;

		HMODULE module = nullptr;
		HostApi api{};
		LifecycleState state = LifecycleState::Unavailable;
		std::string vfsRoot;
		std::string scriptsFile;
		std::string contentFile;
		std::string auxiliaryRoot;
		std::string reportDirectory;
		std::uint32_t initializationFlags = 0;
		bool runtimeStarted = false;
		std::uint64_t frameNumber = 0;
		double simulationTimeSeconds = 0.0;
		std::uint32_t handleGeneration = 0;
		void* handledPlayer = nullptr;
		std::vector<void*> handledCells;
		struct HarnessMutation { StatKind kind; std::uint32_t index; StatField field; double originalValue; };
		std::vector<HarnessMutation> harnessMutations;
		bool restoringHarnessMutations = false;
	};

}
