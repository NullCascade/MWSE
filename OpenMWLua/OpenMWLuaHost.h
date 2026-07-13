#pragma once

#include "../SharedSE/OpenMWLuaBridge.h"

namespace mwse::openmw::host {

	class Host {
	public:
		static Host& getInstance();

		Status initialize(const InitializationConfig* config);
		Status shutdown();
		Status update(const FrameUpdate* update);
		Status queueEvent(const QueuedEvent* eventData);
		Status reload();
		LifecycleState getLifecycleState() const;
		Status getReport(char* buffer, std::uint32_t capacity, std::uint32_t* requiredSize);

	private:
		Host() = default;

		struct Definition {
			std::string contentFile;
			std::string scriptPath;
			std::vector<std::string> flags;
			std::uint32_t lineNumber = 0;
			std::uint32_t order = 0;
		};

		struct ScriptInstance {
			std::uint32_t id = 0;
			std::uint32_t definitionOrder = 0;
			std::uint32_t lineNumber = 0;
			std::string contentFile;
			std::string scriptPath;
			std::string container;
			int environmentReference = LUA_NOREF;
			int interfaceReference = LUA_NOREF;
			std::unordered_map<std::string, int> loadedModules;
			std::unordered_map<std::string, int> timerCallbacks;
			std::unordered_map<std::string, int> engineHandlers;
			std::unordered_map<std::string, int> eventHandlers;
			std::string interfaceName;
			bool hasInterface = false;
			bool mwseGlobalVisible = false;
		};

		struct DelayedEvent {
			std::uint64_t sequence = 0;
			std::string name;
			std::string payload;
			std::string targetContainer;
			int payloadReference = LUA_NOREF;
		};

		struct Timer {
			std::uint64_t sequence = 0;
			std::uint32_t instanceId = 0;
			double dueTime = 0.0;
			bool gameTime = false;
			bool serializable = false;
			std::string callbackName;
			int functionReference = LUA_NOREF;
			int argumentReference = LUA_NOREF;
		};

		struct StorageSubscription {
			std::uint32_t instanceId = 0;
			int callbackReference = LUA_NOREF;
		};

		struct StorageSection {
			std::map<std::string, int> values;
			std::vector<StorageSubscription> subscriptions;
			std::uint32_t lifeTime = 0;
			bool notifying = false;
		};

		Status initializeRuntime();
		void destroyRuntime();
		void parseScriptsFile();
		void parseScriptsText(std::string_view text);
		void validateDefinition(const Definition& definition) const;
		void startContainers();
		void startInstance(const Definition& definition, std::string_view container);
		void createSandbox(ScriptInstance& instance);
		void registerScriptOutput(ScriptInstance& instance);
		ScriptInstance* findInstance(std::uint32_t id) const;
		void callEngineHandlers(std::string_view name, double argument);
		void processTimers();
		void deliverDelayedEvents();
		void callEventHandlers(const DelayedEvent& eventData);
		bool callHandler(ScriptInstance& instance, int functionReference, std::string_view diagnosticName,
			std::optional<double> numberArgument, std::optional<std::string_view> stringArgument);
		int loadSourceModule(ScriptInstance& instance, std::string_view moduleName);
		std::filesystem::path resolveModulePath(std::string_view moduleName) const;
		void pushSafeLibraryClone(const char* name);
		void pushBuiltinPackage(ScriptInstance& instance, std::string_view name);
		void pushUtilPackage(ScriptInstance& instance);
		void pushInterfacesPackage(ScriptInstance& instance);
		void pushAsyncPackage(ScriptInstance& instance);
		void pushStoragePackage(ScriptInstance& instance);
		void pushCorePackage(ScriptInstance& instance);
		void pushSelfPackage(ScriptInstance& instance);
		void pushTypesPackage(ScriptInstance& instance);
		void pushPlayerObject();
		void pushRecordCollection(RecordType type);
		void pushRecord(const RecordSnapshot& snapshot);
		void pushEffect(const EffectSnapshot& snapshot, bool active);
		void pushStatFunctions(lua_State* state, bool includeSkills);
		void pushCoreGameplay();
		void pushReadOnlyProxy(int valueIndex, bool strict);
		void pushStorageSection(ScriptInstance& instance, bool player, std::string_view name, bool readOnly);
		StorageSection& getStorageSection(bool player, std::string_view name);
		void notifyStorage(StorageSection& section, std::string_view sectionName, std::optional<std::string_view> key);
		void recordFoundationProbe(std::string_view name, bool passed);
		void log(LogSeverity severity, std::string_view category, std::string_view message,
			const ScriptInstance* instance = nullptr);
		void writeReportArtifacts();
		std::string buildReport() const;
		std::string buildContainerReport() const;
		std::string buildHandlerReport() const;
		std::string buildBridgeReport() const;
		std::string buildReloadReport() const;
		std::string buildFoundationReport() const;
		std::string buildGameplayReport() const;
		std::string copyBridgeString(StringView value, std::string_view fieldName, bool allowEmpty) const;
		static void* allocator(void* userData, void* pointer, std::size_t oldSize, std::size_t newSize);
		static int requireThunk(lua_State* state);
		static int unsupportedThunk(lua_State* state);
		static int readOnlyNewIndexThunk(lua_State* state);
		static int strictReadOnlyIndexThunk(lua_State* state);
		static int interfaceIndexThunk(lua_State* state);
		static int compatibilityRecordProbeThunk(lua_State* state);
		static int asyncRegisterTimerCallbackThunk(lua_State* state);
		static int asyncNewTimerThunk(lua_State* state);
		static int asyncCallbackThunk(lua_State* state);
		static int asyncCallbackCallThunk(lua_State* state);
		static int storageSectionThunk(lua_State* state);
		static int storageAllSectionsThunk(lua_State* state);
		static int storageGetThunk(lua_State* state);
		static int storageGetCopyThunk(lua_State* state);
		static int storageAsTableThunk(lua_State* state);
		static int storageSubscribeThunk(lua_State* state);
		static int storageSetThunk(lua_State* state);
		static int storageResetThunk(lua_State* state);
		static int storageSetLifeTimeThunk(lua_State* state);
		static int coreTimeThunk(lua_State* state);
		static int coreGetGameSettingThunk(lua_State* state);
		static int coreSendGlobalEventThunk(lua_State* state);
		static int coreL10nThunk(lua_State* state);
		static int coreL10nFormatThunk(lua_State* state);
		static int contentFilesIndexOfThunk(lua_State* state);
		static int contentFilesHasThunk(lua_State* state);
	public:
		static int objectIndexThunk(lua_State* state);
		static int objectNewIndexThunk(lua_State* state);
		static int cellIndexThunk(lua_State* state);
		static int typeObjectIsInstanceThunk(lua_State* state);
		static int statGetterThunk(lua_State* state);
		static int statIndexThunk(lua_State* state);
		static int statNewIndexThunk(lua_State* state);
		static int recordLookupThunk(lua_State* state);
		static int activeSpellsThunk(lua_State* state);
		static int actorSpellsThunk(lua_State* state);
		static int actorSpellMutationThunk(lua_State* state);
		static int birthsignThunk(lua_State* state);
		static int charGenFinishedThunk(lua_State* state);
		static int proxyLenThunk(lua_State* state);
		static int compatibilityInvalidHandleProbeThunk(lua_State* state);
	private:

		LifecycleState mState = LifecycleState::Stopped;
		InitializationConfig mConfig{};
		BridgeCallbacks mCallbacks{};
		std::string mVfsRoot;
		std::string mScriptsFile;
		std::string mContentFile;
		std::string mAuxiliaryRoot;
		std::string mReportDirectory;
		lua_State* mLua = nullptr;
		std::size_t mAllocatedBytes = 0;
		std::uint64_t mRuntimeGeneration = 0;
		std::uint64_t mFrameNumber = 0;
		std::uint64_t mNextEventSequence = 1;
		std::uint64_t mNextTimerSequence = 1;
		std::uint32_t mReloadCount = 0;
		std::vector<Definition> mDefinitions;
		std::vector<std::unique_ptr<ScriptInstance>> mInstances;
		std::vector<DelayedEvent> mPendingEvents;
		std::vector<DelayedEvent> mNextEvents;
		std::vector<Timer> mTimers;
		std::map<std::string, StorageSection> mGlobalStorage;
		std::map<std::string, StorageSection> mPlayerStorage;
		std::map<std::string, bool> mFoundationProbes;
		double mSimulationTimeSeconds = 0.0;
		double mGameTimeSeconds = 0.0;
		double mRealTimeSeconds = 0.0;
		double mRealFrameDuration = 0.0;
		double mSimulationTimeScale = 1.0;
		double mGameTimeScale = 30.0;
		bool mWorldPaused = false;
		std::uint32_t mTimersScheduled = 0;
		std::uint32_t mTimersFired = 0;
		std::uint32_t mStorageNotifications = 0;
		std::uint32_t mInterfaceLookups = 0;
		std::vector<std::string> mEngineHandlerOrder;
		std::vector<std::string> mEventHandlerOrder;
		std::vector<std::string> mDelayedDeliveries;
		std::vector<std::string> mDiagnostics;
		std::string mLastError;
		int mPlayerObjectReference = LUA_NOREF;
		int mTypesPackageReference = LUA_NOREF;
		int mActorTypeReference = LUA_NOREF;
		int mNpcTypeReference = LUA_NOREF;
		int mPlayerTypeReference = LUA_NOREF;
		std::map<std::uint64_t, int> mCellReferences;
		std::uint32_t mGameplayCalls = 0;
		std::uint32_t mRejectedHandles = 0;
	};

}
