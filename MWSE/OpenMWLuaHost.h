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
		static std::string copyLogString(StringView value);
		static std::string getEnvironment(const char* name);
		static bool environmentEnabled(const char* name);

		HMODULE module = nullptr;
		HostApi api{};
		LifecycleState state = LifecycleState::Unavailable;
		std::uint64_t frameNumber = 0;
	};

}
