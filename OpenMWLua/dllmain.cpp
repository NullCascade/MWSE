#include "OpenMWLuaHost.h"

using namespace mwse::openmw;

namespace {
	Status __cdecl initializeHost(const InitializationConfig* config) {
		return host::Host::getInstance().initialize(config);
	}

	Status __cdecl shutdownHost() {
		return host::Host::getInstance().shutdown();
	}

	Status __cdecl updateHost(const FrameUpdate* update) {
		return host::Host::getInstance().update(update);
	}

	Status __cdecl queueHostEvent(const QueuedEvent* eventData) {
		return host::Host::getInstance().queueEvent(eventData);
	}

	Status __cdecl reloadHost() {
		return host::Host::getInstance().reload();
	}

	LifecycleState __cdecl getHostLifecycleState() {
		return host::Host::getInstance().getLifecycleState();
	}

	Status __cdecl getHostReport(char* buffer, std::uint32_t capacity, std::uint32_t* requiredSize) {
		return host::Host::getInstance().getReport(buffer, capacity, requiredSize);
	}
}

extern "C" __declspec(dllexport) Status __cdecl OpenMWLua_QueryApi(
	std::uint32_t requestedAbiVersion, std::uint32_t callerStructureSize, HostApi* api) {
	if (api == nullptr) return Status::InvalidArgument;
	if (requestedAbiVersion != BridgeAbiVersion) return Status::AbiMismatch;
	if (callerStructureSize != sizeof(HostApi)) return Status::StructureSizeMismatch;
	static constexpr char RuntimeVersion[] = LUAJIT_VERSION;
	*api = {
		sizeof(HostApi),
		BridgeAbiVersion,
		BridgeVersion,
		OpenMWApiRevision,
		CapabilityIsolatedLuaState | CapabilitySandboxedSourceModules | CapabilityMenuContainer
			| CapabilityGlobalContainer | CapabilityPlayerContainer | CapabilityDelayedEvents | CapabilityReload
			| CapabilityFoundationUtil | CapabilityFoundationInterfaces | CapabilityFoundationAsync
			| CapabilityFoundationStorage | CapabilityFoundationCore | CapabilityFoundationSelf,
		{ RuntimeVersion, sizeof(RuntimeVersion) - 1 },
		&initializeHost,
		&shutdownHost,
		&updateHost,
		&queueHostEvent,
		&reloadHost,
		&getHostLifecycleState,
		&getHostReport,
	};
	return Status::Ok;
}

BOOL WINAPI DllMain(HINSTANCE instance, DWORD reason, LPVOID) {
	if (reason == DLL_PROCESS_ATTACH) DisableThreadLibraryCalls(instance);
	return TRUE;
}
