#pragma once

#include <cstdint>

namespace mwse::openmw {

	constexpr std::uint32_t BridgeAbiVersion = 4;
	constexpr std::uint32_t BridgeVersion = 4;
	constexpr std::uint32_t OpenMWApiRevision = 70;
	constexpr std::uint32_t MaxBridgeStringLength = 32 * 1024;
	constexpr std::uint32_t BridgeTextCapacity = 128;
	constexpr std::uint32_t MaxRecordItems = 64;
	constexpr std::uint32_t MaxRecordEffects = 8;

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
		InvalidHandle = 11,
		StaleHandle = 12,
		WrongHandleType = 13,
		OutOfRange = 14,
		NotFound = 15,
		ReadOnly = 16,
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
		CapabilityFoundationUtil = 1ull << 7,
		CapabilityFoundationInterfaces = 1ull << 8,
		CapabilityFoundationAsync = 1ull << 9,
		CapabilityFoundationStorage = 1ull << 10,
		CapabilityFoundationCore = 1ull << 11,
		CapabilityFoundationSelf = 1ull << 12,
		CapabilityPlayerBindings = 1ull << 13,
		CapabilityRecordBindings = 1ull << 14,
		CapabilityMutableStats = 1ull << 15,
		CapabilityInputActions = 1ull << 16,
		CapabilityLifecycleHandlers = 1ull << 17,
		CapabilityNativeEngineEvents = 1ull << 18,
		CapabilityPlayerLocalEvents = 1ull << 19,
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

	enum class HandleType : std::uint32_t {
		None = 0,
		Player = 1,
		Cell = 2,
	};

	struct BridgeText {
		std::uint32_t size;
		char data[BridgeTextCapacity];
	};

	enum class ObjectType : std::uint32_t {
		None = 0,
		Actor = 1u << 0,
		Npc = 1u << 1,
		Player = 1u << 2,
	};

	struct ObjectSnapshot {
		std::uint32_t structureSize;
		std::uint32_t abiVersion;
		OpaqueHandle object;
		OpaqueHandle cell;
		std::uint32_t typeMask;
		std::uint32_t reserved;
		BridgeText recordId;
	};

	enum CellPropertyFlag : std::uint32_t {
		CellHasWater = 1u << 0,
		CellHasSky = 1u << 1,
		CellIsExterior = 1u << 2,
		CellIsQuasiExterior = 1u << 3,
		CellHasWaterLevel = 1u << 4,
	};

	struct CellSnapshot {
		std::uint32_t structureSize;
		std::uint32_t abiVersion;
		OpaqueHandle handle;
		std::uint32_t flags;
		std::int32_t gridX;
		std::int32_t gridY;
		double waterLevel;
		BridgeText id;
		BridgeText name;
		BridgeText displayName;
		BridgeText region;
		BridgeText worldSpaceId;
	};

	enum class StatKind : std::uint32_t {
		Attribute = 1,
		Skill = 2,
		Level = 3,
		Health = 4,
	};

	enum class StatField : std::uint32_t {
		Base = 0,
		Current = 1,
		Modified = 2,
		Modifier = 3,
		Damage = 4,
		Progress = 5,
	};

	struct StatSnapshot {
		std::uint32_t structureSize;
		std::uint32_t abiVersion;
		double base;
		double current;
		double modified;
		double modifier;
		double damage;
		double progress;
		std::uint32_t writableMask;
		std::uint32_t reserved;
	};

	enum class RecordType : std::uint32_t {
		Attribute = 1,
		Skill = 2,
		Npc = 3,
		Class = 4,
		Race = 5,
		Birthsign = 6,
		Spell = 7,
		MagicEffect = 8,
	};

	struct EffectSnapshot {
		BridgeText id;
		BridgeText affectedSkill;
		BridgeText affectedAttribute;
		double magnitudeMin;
		double magnitudeMax;
		double magnitudeThisFrame;
		double duration;
		double area;
		double baseCost;
		std::uint32_t range;
		std::uint32_t school;
		std::uint32_t flags;
		std::uint32_t reserved;
	};

	struct RecordSnapshot {
		std::uint32_t structureSize;
		std::uint32_t abiVersion;
		RecordType type;
		std::uint32_t index;
		BridgeText id;
		BridgeText name;
		BridgeText auxiliaryId1;
		BridgeText auxiliaryId2;
		std::uint32_t flags;
		std::int32_t specialization;
		std::int32_t value;
		std::uint32_t itemCount[4];
		BridgeText itemIds[MaxRecordItems];
		double itemValues[MaxRecordItems * 2];
		EffectSnapshot effects[MaxRecordEffects];
	};

	struct ActiveSpellSnapshot {
		std::uint32_t structureSize;
		std::uint32_t abiVersion;
		BridgeText id;
		std::uint32_t affectsBaseValues;
		std::uint32_t effectCount;
		EffectSnapshot effects[MaxRecordEffects];
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

	enum class ValueType : std::uint32_t {
		None = 0,
		Number = 1,
		String = 2,
		Boolean = 3,
	};

	struct BridgeValue {
		std::uint32_t structureSize;
		std::uint32_t abiVersion;
		ValueType type;
		std::uint32_t reserved;
		double number;
		StringView string;
	};

	using GetGameSettingCallback = Status(__cdecl*)(void* userData, StringView name, BridgeValue* value);
	using GetContentFileCountCallback = std::uint32_t(__cdecl*)(void* userData);
	using GetContentFileCallback = Status(__cdecl*)(void* userData, std::uint32_t index, StringView* contentFile);
	using GetPlayerObjectCallback = Status(__cdecl*)(void* userData, ObjectSnapshot* snapshot);
	using ValidateHandleCallback = Status(__cdecl*)(void* userData, OpaqueHandle handle, HandleType expectedType);
	using GetCellCallback = Status(__cdecl*)(void* userData, OpaqueHandle handle, CellSnapshot* snapshot);
	using GetStatCallback = Status(__cdecl*)(void* userData, OpaqueHandle actor, StatKind kind,
		std::uint32_t index, StatSnapshot* snapshot);
	using SetStatCallback = Status(__cdecl*)(void* userData, OpaqueHandle actor, StatKind kind,
		std::uint32_t index, StatField field, double value);
	using GetRecordCountCallback = Status(__cdecl*)(void* userData, RecordType type, std::uint32_t* count);
	using GetRecordCallback = Status(__cdecl*)(void* userData, RecordType type, std::uint32_t index,
		StringView id, RecordSnapshot* snapshot);
	using GetActorSpellCountCallback = Status(__cdecl*)(void* userData, OpaqueHandle actor, std::uint32_t* count);
	using GetActorSpellCallback = Status(__cdecl*)(void* userData, OpaqueHandle actor, std::uint32_t index,
		RecordSnapshot* snapshot);
	using SetActorSpellCallback = Status(__cdecl*)(void* userData, OpaqueHandle actor, StringView id, std::uint32_t add);
	using GetActiveSpellCountCallback = Status(__cdecl*)(void* userData, OpaqueHandle actor, std::uint32_t* count);
	using GetActiveSpellCallback = Status(__cdecl*)(void* userData, OpaqueHandle actor, std::uint32_t index,
		ActiveSpellSnapshot* snapshot);

	struct BridgeCallbacks {
		std::uint32_t structureSize;
		std::uint32_t abiVersion;
		LogCallback log;
		void* logUserData;
		GetGameSettingCallback getGameSetting;
		GetContentFileCountCallback getContentFileCount;
		GetContentFileCallback getContentFile;
		void* gameUserData;
		GetPlayerObjectCallback getPlayerObject;
		ValidateHandleCallback validateHandle;
		GetCellCallback getCell;
		GetStatCallback getStat;
		SetStatCallback setStat;
		GetRecordCountCallback getRecordCount;
		GetRecordCallback getRecord;
		GetActorSpellCountCallback getActorSpellCount;
		GetActorSpellCallback getActorSpell;
		SetActorSpellCallback setActorSpell;
		GetActiveSpellCountCallback getActiveSpellCount;
		GetActiveSpellCallback getActiveSpell;
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
		double simulationDeltaSeconds;
		double simulationTimeSeconds;
		double gameTimeHours;
		double simulationTimeScale;
		double gameTimeScale;
		std::uint32_t paused;
		std::uint32_t reserved;
	};

	enum class NativeEventType : std::uint32_t {
		UiModeChanged = 1,
		PlayerDied = 2,
		SkillLevelUp = 3,
	};

	struct NativeEvent {
		std::uint32_t structureSize;
		std::uint32_t abiVersion;
		NativeEventType type;
		std::uint32_t flags;
		std::uint64_t sequence;
		std::uint32_t index;
		std::uint32_t reserved;
		double value;
		BridgeText name;
		BridgeText previous;
		BridgeText source;
	};

	enum class ActionType : std::uint32_t {
		Boolean = 1,
		Number = 2,
		Range = 3,
	};

	struct ActionUpdate {
		std::uint32_t structureSize;
		std::uint32_t abiVersion;
		ActionType type;
		std::uint32_t reserved;
		StringView key;
		double value;
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
	using QueueNativeEventFunction = Status(__cdecl*)(const NativeEvent* eventData);
	using UpdateActionFunction = Status(__cdecl*)(const ActionUpdate* updateData);
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
		QueueNativeEventFunction queueNativeEvent;
		UpdateActionFunction updateAction;
	};

	using QueryApiFunction = Status(__cdecl*)(std::uint32_t requestedAbiVersion,
		std::uint32_t callerStructureSize, HostApi* api);

}
