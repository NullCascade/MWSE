#include "OpenMWLuaHost.h"

#include "Log.h"

#include "TES3DataHandler.h"
#include "TES3GameFile.h"
#include "TES3GameSetting.h"
#include "TES3GlobalVariable.h"
#include "TES3MobilePlayer.h"
#include "TES3WorldController.h"

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

	bool HostController::isGameDataReady() const {
		auto* dataHandler = TES3::DataHandler::get();
		auto* world = TES3::WorldController::get();
		auto* player = world ? world->getMobilePlayer() : nullptr;
		return dataHandler != nullptr && dataHandler->nonDynamicData != nullptr
			&& dataHandler->nonDynamicData->GMSTs != nullptr
			&& dataHandler->nonDynamicData->GMSTs[TES3::GMST::sHealth] != nullptr
			&& player != nullptr && player->getCell() != nullptr;
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

	Status __cdecl HostController::getGameSetting(void*, StringView name, BridgeValue* value) {
		if (value == nullptr || value->structureSize != sizeof(BridgeValue) || value->abiVersion != BridgeAbiVersion
			|| name.size == 0 || name.size > MaxBridgeStringLength || name.data == nullptr) {
			return Status::InvalidArgument;
		}
		const std::string settingName(name.data, name.size);
		auto* dataHandler = TES3::DataHandler::get();
		if (dataHandler == nullptr || dataHandler->nonDynamicData == nullptr) return Status::InvalidState;
		for (int index = TES3::GMST::sMonthMorningstar; index <= TES3::GMST::sWitchhunter; ++index) {
			auto* info = TES3::GameSettingInfo::get(index);
			if (info == nullptr || info->name == nullptr || _stricmp(info->name, settingName.c_str()) != 0) continue;
			auto* setting = dataHandler->nonDynamicData->GMSTs[index];
			if (setting == nullptr) return Status::InvalidState;
			value->string = {};
			switch (setting->getType()) {
			case 'i': value->type = ValueType::Number; value->number = setting->value.asLong; return Status::Ok;
			case 'f': value->type = ValueType::Number; value->number = setting->value.asFloat; return Status::Ok;
			case 's': {
				const char* text = setting->value.asString;
				if (text == nullptr) return Status::InvalidState;
				const std::size_t size = strnlen(text, MaxBridgeStringLength + 1);
				if (size > MaxBridgeStringLength) return Status::InvalidArgument;
				value->type = ValueType::String;
				value->string = { text, static_cast<std::uint32_t>(size) };
				return Status::Ok;
			}
			default: return Status::Unsupported;
			}
		}
		value->type = ValueType::None;
		value->string = {};
		value->number = 0.0;
		return Status::Ok;
	}

	std::uint32_t __cdecl HostController::getContentFileCount(void*) {
		auto* dataHandler = TES3::DataHandler::get();
		if (dataHandler == nullptr || dataHandler->nonDynamicData == nullptr) return 0;
		return static_cast<std::uint32_t>(dataHandler->nonDynamicData->getActiveMods().size());
	}

	Status __cdecl HostController::getContentFile(void*, std::uint32_t index, StringView* contentFile) {
		if (contentFile == nullptr) return Status::InvalidArgument;
		auto* dataHandler = TES3::DataHandler::get();
		if (dataHandler == nullptr || dataHandler->nonDynamicData == nullptr) return Status::InvalidState;
		const auto activeMods = dataHandler->nonDynamicData->getActiveMods();
		if (index >= activeMods.size() || activeMods[index] == nullptr) return Status::InvalidArgument;
		auto* selected = activeMods[index];
		const std::size_t size = strnlen(selected->filename, sizeof(selected->filename));
		if (size == sizeof(selected->filename) || size > MaxBridgeStringLength) return Status::InvalidArgument;
		*contentFile = { selected->filename, static_cast<std::uint32_t>(size) };
		return Status::Ok;
	}

	Status HostController::startRuntime() {
		resetHandles();
		InitializationConfig config{};
		config.structureSize = sizeof(config);
		config.abiVersion = BridgeAbiVersion;
		config.flags = initializationFlags;
		config.callbacks = { sizeof(BridgeCallbacks), BridgeAbiVersion, &receiveLog, this,
			&getGameSetting, &getContentFileCount, &getContentFile, this,
			&getPlayerObject, &validateHandle, &getCell, &getStat, &setStat, &getRecordCount, &getRecord,
			&getActorSpellCount, &getActorSpell, &setActorSpell, &getActiveSpellCount, &getActiveSpell };
		config.vfsRoot = { vfsRoot.data(), static_cast<std::uint32_t>(vfsRoot.size()) };
		config.scriptsFile = { scriptsFile.data(), static_cast<std::uint32_t>(scriptsFile.size()) };
		config.contentFile = { contentFile.data(), static_cast<std::uint32_t>(contentFile.size()) };
		config.auxiliaryRoot = { auxiliaryRoot.data(), static_cast<std::uint32_t>(auxiliaryRoot.size()) };
		config.reportDirectory = { reportDirectory.data(), static_cast<std::uint32_t>(reportDirectory.size()) };
		const Status status = api.initialize(&config);
		runtimeStarted = true;
		state = api.getLifecycleState();
		log::getLog() << "[OpenMW Lua] runtime=\"" << copyLogString(api.runtimeVersion) << "\" apiRevision="
			<< api.apiRevision << " bridgeVersion=" << api.bridgeVersion << " abiVersion=" << api.abiVersion
			<< " capabilities=0x" << std::hex << api.capabilities << std::dec << " state="
			<< static_cast<std::uint32_t>(state) << " status=" << static_cast<std::uint32_t>(status) << std::endl;
		if (status != Status::Ok) {
			log::getLog() << "[OpenMW Lua] Host initialization failed safely. MWSE Lua remains active." << std::endl;
		}
		return status;
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

		vfsRoot = getEnvironment("MWSE_OPENMW_LUA_VFS_ROOT");
		if (vfsRoot.empty()) vfsRoot = (std::filesystem::current_path() / "Data Files").string();
		scriptsFile = getEnvironment("MWSE_OPENMW_LUA_SCRIPTS_FILE");
		contentFile = getEnvironment("MWSE_OPENMW_LUA_CONTENT_FILE");
		if (contentFile.empty() && !scriptsFile.empty()) contentFile = std::filesystem::path(scriptsFile).filename().string();
		auxiliaryRoot = (std::filesystem::current_path() / "Data Files" / "MWSE" / "core" / "openmw").string();
		reportDirectory = getEnvironment("MWSE_OPENMW_LUA_REPORT_DIRECTORY");
		initializationFlags = environmentEnabled("MWSE_OPENMW_LUA_DISABLED") ? 0 : InitializationEnabled;
		if (environmentEnabled("MWSE_OPENMW_LUA_HARNESS")) initializationFlags |= InitializationHarnessMode;
		state = LifecycleState::Initializing;
		log::getLog() << "[OpenMW Lua] Support DLL loaded and bridge ABI accepted; runtime startup is waiting for game data." << std::endl;
	}

	void HostController::shutdown() {
		if (module == nullptr || api.shutdown == nullptr) return;
		if (runtimeStarted) api.shutdown();
		runtimeStarted = false;
		resetHandles();
		state = LifecycleState::Stopped;
	}

	void HostController::update(double deltaSeconds, double simulationTimeSeconds, bool paused) {
		if (module == nullptr || api.update == nullptr || api.getLifecycleState == nullptr) return;
		if (!runtimeStarted) {
			if ((initializationFlags & InitializationEnabled) != 0 && !isGameDataReady()) return;
			if (startRuntime() != Status::Ok) return;
		}
		state = api.getLifecycleState();
		if (state != LifecycleState::Running) return;
		auto* currentPlayer = TES3::WorldController::get() ? TES3::WorldController::get()->getMobilePlayer() : nullptr;
		if (currentPlayer != handledPlayer) {
			resetHandles();
			if (api.reload() != Status::Ok) { state = api.getLifecycleState(); return; }
		}
		const double realDeltaSeconds = TES3::WorldController::realDeltaTime;
		if (!paused) this->simulationTimeSeconds += deltaSeconds;
		auto* worldController = TES3::WorldController::get();
		const double gameTimeScale = worldController != nullptr && worldController->gvarTimescale != nullptr
			? worldController->gvarTimescale->value : 0.0;
		const double simulationTimeScale = realDeltaSeconds > 0.0 ? deltaSeconds / realDeltaSeconds : 0.0;
		FrameUpdate updateData{ sizeof(FrameUpdate), BridgeAbiVersion, ++frameNumber, realDeltaSeconds,
			this->simulationTimeSeconds, simulationTimeSeconds, simulationTimeScale, gameTimeScale,
			paused ? 1u : 0u, 0 };
		api.update(&updateData);
	}

	bool HostController::reload() {
		if (module == nullptr || api.reload == nullptr || !runtimeStarted) return false;
		resetHandles();
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
