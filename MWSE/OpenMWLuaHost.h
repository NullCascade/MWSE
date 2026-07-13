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
		static std::string copyLogString(StringView value);
		static std::string getEnvironment(const char* name);
		static bool environmentEnabled(const char* name);
		bool isGameDataReady() const;
		Status startRuntime();

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
	};

}
