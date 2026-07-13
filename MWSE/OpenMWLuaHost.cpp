#include "OpenMWLuaHost.h"

#include "Log.h"

namespace mwse::openmw {

	HostController& HostController::getInstance() {
		static HostController instance;
		return instance;
	}

	std::string HostController::getEnvironment(const char* name) {
		const char* value = std::getenv(name);
		return value == nullptr ? std::string() : std::string(value);
	}

	bool HostController::environmentEnabled(const char* name) {
		const std::string value = getEnvironment(name);
		return value == "1" || value == "true" || value == "TRUE";
	}

	std::string HostController::copyLogString(StringView value) {
		if (value.size > MaxBridgeStringLength || (value.size != 0 && value.data == nullptr)) return "<invalid-bridge-string>";
		return value.size == 0 ? std::string() : std::string(value.data, value.size);
	}

	void __cdecl HostController::receiveLog(void*, const LogMessage* message) {
		if (message == nullptr || message->structureSize != sizeof(LogMessage) || message->abiVersion != BridgeAbiVersion) {
			log::getLog() << "[OpenMW Lua] Rejected malformed structured log callback." << std::endl;
			return;
		}
		log::getLog() << "[OpenMW Lua] severity=" << static_cast<std::uint32_t>(message->severity)
			<< " category=\"" << copyLogString(message->category) << "\""
			<< " content=\"" << copyLogString(message->contentFile) << "\""
			<< " script=\"" << copyLogString(message->scriptPath) << "\""
			<< " container=\"" << copyLogString(message->container) << "\" "
			<< copyLogString(message->message) << std::endl;
	}

	void HostController::initialize() {
		if (module != nullptr) return;
		const std::filesystem::path path = std::filesystem::current_path() / "Data Files" / "MWSE" / "core" / "lib" / "openmw-lua.dll";
		module = LoadLibraryW(path.wstring().c_str());
		if (module == nullptr) {
			state = LifecycleState::Unavailable;
			log::getLog() << "[OpenMW Lua] Support DLL unavailable (error " << GetLastError()
				<< "). MWSE Lua will continue normally." << std::endl;
			return;
		}

		auto query = reinterpret_cast<QueryApiFunction>(GetProcAddress(module, "OpenMWLua_QueryApi"));
		if (query == nullptr) {
			state = LifecycleState::Incompatible;
			log::getLog() << "[OpenMW Lua] Support DLL has no bridge entry point. MWSE Lua will continue normally." << std::endl;
			return;
		}
		Status status = query(BridgeAbiVersion, sizeof(HostApi), &api);
		if (status != Status::Ok || api.structureSize != sizeof(HostApi) || api.abiVersion != BridgeAbiVersion
			|| api.initialize == nullptr || api.shutdown == nullptr || api.update == nullptr || api.queueEvent == nullptr
			|| api.reload == nullptr || api.getLifecycleState == nullptr || api.getReport == nullptr
			|| api.runtimeVersion.size > MaxBridgeStringLength || (api.runtimeVersion.size != 0 && api.runtimeVersion.data == nullptr)) {
			state = LifecycleState::Incompatible;
			log::getLog() << "[OpenMW Lua] Bridge ABI or structure validation failed (status "
				<< static_cast<std::uint32_t>(status) << "). MWSE Lua will continue normally." << std::endl;
			return;
		}

		std::string vfsRoot = getEnvironment("MWSE_OPENMW_LUA_VFS_ROOT");
		if (vfsRoot.empty()) vfsRoot = (std::filesystem::current_path() / "Data Files").string();
		std::string scriptsFile = getEnvironment("MWSE_OPENMW_LUA_SCRIPTS_FILE");
		std::string contentFile = getEnvironment("MWSE_OPENMW_LUA_CONTENT_FILE");
		if (contentFile.empty() && !scriptsFile.empty()) contentFile = std::filesystem::path(scriptsFile).filename().string();
		std::string auxiliaryRoot = (std::filesystem::current_path() / "Data Files" / "MWSE" / "core" / "openmw").string();
		std::string reportDirectory = getEnvironment("MWSE_OPENMW_LUA_REPORT_DIRECTORY");
		std::uint32_t flags = environmentEnabled("MWSE_OPENMW_LUA_DISABLED") ? 0 : InitializationEnabled;
		if (environmentEnabled("MWSE_OPENMW_LUA_HARNESS")) flags |= InitializationHarnessMode;

		InitializationConfig config{};
		config.structureSize = sizeof(config);
		config.abiVersion = BridgeAbiVersion;
		config.flags = flags;
		config.callbacks = { sizeof(BridgeCallbacks), BridgeAbiVersion, &receiveLog, this };
		config.vfsRoot = { vfsRoot.data(), static_cast<std::uint32_t>(vfsRoot.size()) };
		config.scriptsFile = { scriptsFile.data(), static_cast<std::uint32_t>(scriptsFile.size()) };
		config.contentFile = { contentFile.data(), static_cast<std::uint32_t>(contentFile.size()) };
		config.auxiliaryRoot = { auxiliaryRoot.data(), static_cast<std::uint32_t>(auxiliaryRoot.size()) };
		config.reportDirectory = { reportDirectory.data(), static_cast<std::uint32_t>(reportDirectory.size()) };
		status = api.initialize(&config);
		state = api.getLifecycleState();
		log::getLog() << "[OpenMW Lua] runtime=\"" << copyLogString(api.runtimeVersion) << "\" apiRevision="
			<< api.apiRevision << " bridgeVersion=" << api.bridgeVersion << " abiVersion=" << api.abiVersion
			<< " capabilities=0x" << std::hex << api.capabilities << std::dec << " state="
			<< static_cast<std::uint32_t>(state) << " status=" << static_cast<std::uint32_t>(status) << std::endl;
		if (status != Status::Ok) {
			log::getLog() << "[OpenMW Lua] Host initialization failed safely. MWSE Lua remains active." << std::endl;
		}
	}

	void HostController::shutdown() {
		if (module == nullptr || api.shutdown == nullptr) return;
		api.shutdown();
		state = LifecycleState::Stopped;
	}

	void HostController::update(double deltaSeconds, double simulationTimeSeconds, bool paused) {
		if (module == nullptr || api.update == nullptr || api.getLifecycleState == nullptr) return;
		state = api.getLifecycleState();
		if (state != LifecycleState::Running) return;
		FrameUpdate updateData{ sizeof(FrameUpdate), BridgeAbiVersion, ++frameNumber, deltaSeconds,
			simulationTimeSeconds, 0.0, paused ? 1u : 0u, 0 };
		api.update(&updateData);
	}

	bool HostController::reload() {
		if (module == nullptr || api.reload == nullptr) return false;
		const Status status = api.reload();
		state = api.getLifecycleState();
		return status == Status::Ok && state == LifecycleState::Running;
	}

	std::string HostController::getReport() const {
		if (module == nullptr || api.getReport == nullptr) return {};
		std::uint32_t required = 0;
		Status status = api.getReport(nullptr, 0, &required);
		if (status != Status::BufferTooSmall || required == 0 || required > 16 * 1024 * 1024) return {};
		std::string report(required, '\0');
		status = api.getReport(report.data(), required, &required);
		if (status != Status::Ok) return {};
		if (!report.empty() && report.back() == '\0') report.pop_back();
		return report;
	}

	LifecycleState HostController::getLifecycleState() const {
		return module != nullptr && api.getLifecycleState != nullptr ? api.getLifecycleState() : state;
	}

	bool HostController::isLoaded() const {
		return module != nullptr && api.getLifecycleState != nullptr;
	}

}
