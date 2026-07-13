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
			std::unordered_map<std::string, int> loadedModules;
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
		void callEngineHandlers(std::string_view name, double argument);
		void deliverDelayedEvents();
		void callEventHandlers(const DelayedEvent& eventData);
		bool callHandler(ScriptInstance& instance, int functionReference, std::string_view diagnosticName,
			std::optional<double> numberArgument, std::optional<std::string_view> stringArgument);
		int loadSourceModule(ScriptInstance& instance, std::string_view moduleName);
		std::filesystem::path resolveModulePath(std::string_view moduleName) const;
		void pushSafeLibraryClone(const char* name);
		void pushBuiltinPackage(std::string_view name);
		void log(LogSeverity severity, std::string_view category, std::string_view message,
			const ScriptInstance* instance = nullptr);
		void writeReportArtifacts();
		std::string buildReport() const;
		std::string buildContainerReport() const;
		std::string buildHandlerReport() const;
		std::string buildBridgeReport() const;
		std::string buildReloadReport() const;
		std::string copyBridgeString(StringView value, std::string_view fieldName, bool allowEmpty) const;
		static void* allocator(void* userData, void* pointer, std::size_t oldSize, std::size_t newSize);
		static int requireThunk(lua_State* state);
		static int unsupportedThunk(lua_State* state);

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
		std::uint32_t mReloadCount = 0;
		std::vector<Definition> mDefinitions;
		std::vector<std::unique_ptr<ScriptInstance>> mInstances;
		std::vector<DelayedEvent> mPendingEvents;
		std::vector<DelayedEvent> mNextEvents;
		std::vector<std::string> mEngineHandlerOrder;
		std::vector<std::string> mEventHandlerOrder;
		std::vector<std::string> mDelayedDeliveries;
		std::vector<std::string> mDiagnostics;
		std::string mLastError;
	};

}
