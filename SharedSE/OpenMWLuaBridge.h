#pragma once

#include <cstdint>

namespace mwse::openmw {

	constexpr std::uint32_t BridgeAbiVersion = 1;
	constexpr std::uint32_t BridgeVersion = 1;
	constexpr std::uint32_t OpenMWApiRevision = 70;
	constexpr std::uint32_t MaxBridgeStringLength = 32 * 1024;

	enum class Status : std::uint32_t {
		Ok = 0,
		InvalidArgument = 1,
		AbiMismatch = 2,
		StructureSizeMismatch = 3,
		MissingCallback = 4,
		InvalidState = 5,
		IoError = 6,
		ParseError = 7,
		LuaError = 8,
		Unsupported = 9,
		BufferTooSmall = 10,
	};

	enum class LifecycleState : std::uint32_t {
		Unavailable = 0,
		Disabled = 1,
		Initializing = 2,
		Running = 3,
		Failed = 4,
		ShuttingDown = 5,
		Stopped = 6,
		Incompatible = 7,
	};

	enum class LogSeverity : std::uint32_t {
		Debug = 0,
		Info = 1,
		Warning = 2,
		Error = 3,
	};

	enum Capability : std::uint64_t {
		CapabilityIsolatedLuaState = 1ull << 0,
		CapabilitySandboxedSourceModules = 1ull << 1,
		CapabilityMenuContainer = 1ull << 2,
		CapabilityGlobalContainer = 1ull << 3,
		CapabilityPlayerContainer = 1ull << 4,
		CapabilityDelayedEvents = 1ull << 5,
		CapabilityReload = 1ull << 6,
	};

	struct StringView {
		const char* data;
		std::uint32_t size;
	};

	struct OpaqueHandle {
		std::uint64_t value;
		std::uint32_t generation;
		std::uint32_t reserved;
	};

	struct LogMessage {
		std::uint32_t structureSize;
		std::uint32_t abiVersion;
		LogSeverity severity;
		std::uint32_t reserved;
		StringView category;
		StringView contentFile;
		StringView scriptPath;
		StringView container;
		StringView message;
	};

	using LogCallback = void(__cdecl*)(void* userData, const LogMessage* message);

	struct BridgeCallbacks {
		std::uint32_t structureSize;
		std::uint32_t abiVersion;
		LogCallback log;
		void* logUserData;
	};

	enum InitializationFlag : std::uint32_t {
		InitializationEnabled = 1u << 0,
		InitializationHarnessMode = 1u << 1,
		InitializationParseOnly = 1u << 2,
	};

	struct InitializationConfig {
		std::uint32_t structureSize;
		std::uint32_t abiVersion;
		std::uint32_t flags;
		std::uint32_t reserved;
		BridgeCallbacks callbacks;
		StringView vfsRoot;
		StringView scriptsFile;
		StringView contentFile;
		StringView auxiliaryRoot;
		StringView reportDirectory;
	};

	struct FrameUpdate {
		std::uint32_t structureSize;
		std::uint32_t abiVersion;
		std::uint64_t frameNumber;
		double realDeltaSeconds;
		double simulationTimeSeconds;
		double gameTimeHours;
		std::uint32_t paused;
		std::uint32_t reserved;
	};

	struct QueuedEvent {
		std::uint32_t structureSize;
		std::uint32_t abiVersion;
		StringView name;
		StringView serializedPayload;
	};

	using InitializeFunction = Status(__cdecl*)(const InitializationConfig* config);
	using ShutdownFunction = Status(__cdecl*)();
	using UpdateFunction = Status(__cdecl*)(const FrameUpdate* update);
	using QueueEventFunction = Status(__cdecl*)(const QueuedEvent* eventData);
	using ReloadFunction = Status(__cdecl*)();
	using GetLifecycleStateFunction = LifecycleState(__cdecl*)();
	using GetReportFunction = Status(__cdecl*)(char* buffer, std::uint32_t capacity, std::uint32_t* requiredSize);

	struct HostApi {
		std::uint32_t structureSize;
		std::uint32_t abiVersion;
		std::uint32_t bridgeVersion;
		std::uint32_t apiRevision;
		std::uint64_t capabilities;
		StringView runtimeVersion;
		InitializeFunction initialize;
		ShutdownFunction shutdown;
		UpdateFunction update;
		QueueEventFunction queueEvent;
		ReloadFunction reload;
		GetLifecycleStateFunction getLifecycleState;
		GetReportFunction getReport;
	};

	using QueryApiFunction = Status(__cdecl*)(std::uint32_t requestedAbiVersion,
		std::uint32_t callerStructureSize, HostApi* api);

}
