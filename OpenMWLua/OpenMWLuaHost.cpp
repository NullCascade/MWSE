#include "OpenMWLuaHost.h"

namespace mwse::openmw::host {

	namespace {
		constexpr std::size_t MaxScriptsFileSize = 1024 * 1024;
		constexpr std::size_t MaxScriptSize = 4 * 1024 * 1024;
		constexpr std::size_t MaxLineSize = 4096;

		std::string trim(std::string_view value) {
			while (!value.empty() && std::isspace(static_cast<unsigned char>(value.front()))) {
				value.remove_prefix(1);
			}
			while (!value.empty() && std::isspace(static_cast<unsigned char>(value.back()))) {
				value.remove_suffix(1);
			}
			return std::string(value);
		}

		std::string upper(std::string_view value) {
			std::string result(value);
			std::transform(result.begin(), result.end(), result.begin(), [](unsigned char c) {
				return static_cast<char>(std::toupper(c));
			});
			return result;
		}

		std::string lower(std::string_view value) {
			std::string result(value);
			std::transform(result.begin(), result.end(), result.begin(), [](unsigned char c) {
				return static_cast<char>(std::tolower(c));
			});
			return result;
		}

		std::string jsonEscape(std::string_view value) {
			std::ostringstream stream;
			for (unsigned char c : value) {
				switch (c) {
				case '\\': stream << "\\\\"; break;
				case '"': stream << "\\\""; break;
				case '\b': stream << "\\b"; break;
				case '\f': stream << "\\f"; break;
				case '\n': stream << "\\n"; break;
				case '\r': stream << "\\r"; break;
				case '\t': stream << "\\t"; break;
				default:
					if (c < 0x20) {
						stream << "\\u" << std::hex << std::setw(4) << std::setfill('0') << static_cast<int>(c);
					}
					else {
						stream << static_cast<char>(c);
					}
				}
			}
			return stream.str();
		}

		template <typename T>
		std::string jsonStringArray(const std::vector<T>& values) {
			std::ostringstream stream;
			stream << '[';
			for (std::size_t i = 0; i < values.size(); ++i) {
				if (i != 0) stream << ',';
				stream << '"' << jsonEscape(values[i]) << '"';
			}
			stream << ']';
			return stream.str();
		}

		StringView asBridgeString(std::string_view value) {
			return { value.data(), static_cast<std::uint32_t>(value.size()) };
		}

		void writeAtomic(const std::filesystem::path& path, std::string_view data) {
			std::filesystem::create_directories(path.parent_path());
			auto temporary = path;
			temporary += ".tmp";
			{
				std::ofstream stream(temporary, std::ios::binary | std::ios::trunc);
				if (!stream) throw std::runtime_error("could not open report temporary file");
				stream.write(data.data(), static_cast<std::streamsize>(data.size()));
				if (!stream) throw std::runtime_error("could not write report temporary file");
			}
			std::error_code error;
			std::filesystem::remove(path, error);
			error.clear();
			std::filesystem::rename(temporary, path, error);
			if (error) throw std::runtime_error("could not publish report: " + error.message());
		}

		template <typename DefinitionType>
		bool containsFlag(const DefinitionType& definition, std::string_view flag) {
			return std::find(definition.flags.begin(), definition.flags.end(), flag) != definition.flags.end();
		}
	}

	Host& Host::getInstance() {
		static Host instance;
		return instance;
	}

	std::string Host::copyBridgeString(StringView value, std::string_view fieldName, bool allowEmpty) const {
		if (value.size > MaxBridgeStringLength) {
			throw std::runtime_error(std::string(fieldName) + " exceeds the bridge string bound");
		}
		if (value.size != 0 && value.data == nullptr) {
			throw std::runtime_error(std::string(fieldName) + " has a null pointer with non-zero size");
		}
		if (!allowEmpty && value.size == 0) {
			throw std::runtime_error(std::string(fieldName) + " is required");
		}
		return value.size == 0 ? std::string() : std::string(value.data, value.size);
	}

	Status Host::initialize(const InitializationConfig* config) {
		if (config == nullptr) return Status::InvalidArgument;
		if (config->structureSize != sizeof(InitializationConfig)) return Status::StructureSizeMismatch;
		if (config->abiVersion != BridgeAbiVersion) return Status::AbiMismatch;
		if (config->callbacks.structureSize != sizeof(BridgeCallbacks)) return Status::StructureSizeMismatch;
		if (config->callbacks.abiVersion != BridgeAbiVersion) return Status::AbiMismatch;
		if (config->callbacks.log == nullptr) return Status::MissingCallback;
		if (mState == LifecycleState::Running || mState == LifecycleState::Initializing) return Status::InvalidState;

		mConfig = *config;
		mCallbacks = config->callbacks;
		try {
			mVfsRoot = copyBridgeString(config->vfsRoot, "vfsRoot", false);
			mScriptsFile = copyBridgeString(config->scriptsFile, "scriptsFile", true);
			mContentFile = copyBridgeString(config->contentFile, "contentFile", true);
			mAuxiliaryRoot = copyBridgeString(config->auxiliaryRoot, "auxiliaryRoot", true);
			mReportDirectory = copyBridgeString(config->reportDirectory, "reportDirectory", true);
		}
		catch (const std::exception& error) {
			mState = LifecycleState::Failed;
			mLastError = error.what();
			return Status::InvalidArgument;
		}

		if ((config->flags & InitializationEnabled) == 0) {
			mState = LifecycleState::Disabled;
			log(LogSeverity::Info, "lifecycle", "OpenMW Lua compatibility host is disabled");
			writeReportArtifacts();
			return Status::Ok;
		}
		return initializeRuntime();
	}

	Status Host::initializeRuntime() {
		mState = LifecycleState::Initializing;
		mLastError.clear();
		mDefinitions.clear();
		mInstances.clear();
		mPendingEvents.clear();
		mNextEvents.clear();
		mEngineHandlerOrder.clear();
		mEventHandlerOrder.clear();
		mDelayedDeliveries.clear();
		mDiagnostics.clear();
		mFrameNumber = 0;
		mNextEventSequence = 1;

		try {
			mLua = lua_newstate(&allocator, this);
			if (mLua == nullptr) throw std::runtime_error("failed to create the isolated LuaJIT state");
			luaL_openlibs(mLua);
			++mRuntimeGeneration;
			parseScriptsFile();
			if ((mConfig.flags & InitializationParseOnly) == 0) {
				startContainers();
			}
			mState = LifecycleState::Running;
			std::ostringstream startup;
			startup << "runtime=" << LUAJIT_VERSION << ", apiRevision=" << OpenMWApiRevision
				<< ", bridgeVersion=" << BridgeVersion << ", generation=" << mRuntimeGeneration
				<< ", capabilities=0x" << std::hex
				<< (CapabilityIsolatedLuaState | CapabilitySandboxedSourceModules | CapabilityMenuContainer
					| CapabilityGlobalContainer | CapabilityPlayerContainer | CapabilityDelayedEvents | CapabilityReload);
			log(LogSeverity::Info, "startup", startup.str());
			if ((mConfig.flags & InitializationHarnessMode) != 0 && (mConfig.flags & InitializationParseOnly) == 0) {
				QueuedEvent eventData{ sizeof(QueuedEvent), BridgeAbiVersion, asBridgeString("Milestone3Event"), {} };
				queueEvent(&eventData);
			}
			writeReportArtifacts();
			return Status::Ok;
		}
		catch (const std::exception& error) {
			mLastError = error.what();
			mDiagnostics.push_back(mLastError);
			log(LogSeverity::Error, "initialization", mLastError);
			destroyRuntime();
			mState = LifecycleState::Failed;
			writeReportArtifacts();
			return mLastError.find("Lua") != std::string::npos ? Status::LuaError : Status::ParseError;
		}
	}

	Status Host::shutdown() {
		if (mState == LifecycleState::Stopped || mState == LifecycleState::Unavailable) return Status::Ok;
		mState = LifecycleState::ShuttingDown;
		log(LogSeverity::Info, "lifecycle", "shutting down the OpenMW Lua compatibility runtime");
		// Preserve the last live container and handler reports before releasing Lua references.
		writeReportArtifacts();
		destroyRuntime();
		mState = LifecycleState::Stopped;
		if (!mReportDirectory.empty()) {
			try {
				auto root = std::filesystem::path(mReportDirectory);
				writeAtomic(root / "bridge-runtime-report.json", buildBridgeReport());
				writeAtomic(root / "reload-shutdown-report.json", buildReloadReport());
				writeAtomic(root / "openmw-host-report.json", buildReport());
			}
			catch (...) {}
		}
		return Status::Ok;
	}

	void Host::destroyRuntime() {
		mInstances.clear();
		mPendingEvents.clear();
		mNextEvents.clear();
		if (mLua != nullptr) {
			lua_close(mLua);
			mLua = nullptr;
		}
	}

	void* Host::allocator(void* userData, void* pointer, std::size_t oldSize, std::size_t newSize) {
		auto* self = static_cast<Host*>(userData);
		if (newSize == 0) {
			std::free(pointer);
			self->mAllocatedBytes = self->mAllocatedBytes >= oldSize ? self->mAllocatedBytes - oldSize : 0;
			return nullptr;
		}
		void* result = std::realloc(pointer, newSize);
		if (result != nullptr) {
			self->mAllocatedBytes = self->mAllocatedBytes >= oldSize
				? self->mAllocatedBytes - oldSize + newSize : self->mAllocatedBytes + newSize;
		}
		return result;
	}

	void Host::parseScriptsFile() {
		if (mScriptsFile.empty()) return;
		std::filesystem::path path(mScriptsFile);
		if (!path.is_absolute()) path = std::filesystem::path(mVfsRoot) / path;
		std::error_code error;
		auto size = std::filesystem::file_size(path, error);
		if (error) throw std::runtime_error("content '" + mContentFile + "': cannot read .omwscripts file '" + path.string() + "'");
		if (size > MaxScriptsFileSize) throw std::runtime_error("content '" + mContentFile + "': .omwscripts file exceeds 1 MiB");
		std::ifstream stream(path, std::ios::binary);
		std::string text(std::istreambuf_iterator<char>(stream), {});
		parseScriptsText(text);
	}

	void Host::parseScriptsText(std::string_view text) {
		static const std::set<std::string> knownFlags{
			"GLOBAL", "MENU", "PLAYER", "CUSTOM", "LOAD", "ACTIVATOR", "ARMOR", "BOOK", "CLOTHING",
			"CONTAINER", "CREATURE", "DOOR", "INGREDIENT", "LIGHT", "MISC_ITEM", "NPC", "POTION",
			"WEAPON", "APPARATUS", "LOCKPICK", "PROBE", "REPAIR"
		};
		std::unordered_map<std::string, std::set<std::string>> declarations;
		std::uint32_t lineNumber = 0;
		while (!text.empty()) {
			++lineNumber;
			auto newline = text.find('\n');
			std::string_view raw = text.substr(0, newline);
			text = newline == std::string_view::npos ? std::string_view() : text.substr(newline + 1);
			if (!raw.empty() && raw.back() == '\r') raw.remove_suffix(1);
			if (raw.size() > MaxLineSize) throw std::runtime_error("content '" + mContentFile + "' line " + std::to_string(lineNumber) + ": declaration exceeds 4096 bytes");
			std::string line = trim(raw);
			if (line.empty() || line.front() == '#') continue;
			auto colon = line.find(':');
			if (colon == std::string::npos || line.find(':', colon + 1) != std::string::npos) {
				throw std::runtime_error("content '" + mContentFile + "' line " + std::to_string(lineNumber) + ": malformed declaration");
			}
			std::string flagText = line.substr(0, colon);
			std::string path = trim(std::string_view(line).substr(colon + 1));
			if (path.empty()) throw std::runtime_error("content '" + mContentFile + "' line " + std::to_string(lineNumber) + ": missing script path");
			std::replace(path.begin(), path.end(), '\\', '/');
			Definition definition{ mContentFile, path, {}, lineNumber, static_cast<std::uint32_t>(mDefinitions.size()) };
			std::string token;
			for (std::size_t i = 0; i <= flagText.size(); ++i) {
				char c = i == flagText.size() ? ',' : flagText[i];
				if (c == ',' || std::isspace(static_cast<unsigned char>(c))) {
					if (!token.empty()) {
						std::string flag = upper(token);
						if (!knownFlags.contains(flag)) throw std::runtime_error("content '" + mContentFile + "' line " + std::to_string(lineNumber) + ": unknown flag '" + flag + "'");
						if (std::find(definition.flags.begin(), definition.flags.end(), flag) != definition.flags.end()) throw std::runtime_error("content '" + mContentFile + "' line " + std::to_string(lineNumber) + ": duplicate flag '" + flag + "'");
						definition.flags.push_back(flag);
						token.clear();
					}
				}
				else token.push_back(c);
			}
			if (definition.flags.empty()) throw std::runtime_error("content '" + mContentFile + "' line " + std::to_string(lineNumber) + ": no flags found");
			validateDefinition(definition);
			std::set<std::string> current(definition.flags.begin(), definition.flags.end());
			auto [it, inserted] = declarations.emplace(lower(path), current);
			if (!inserted && it->second != current) throw std::runtime_error("content '" + mContentFile + "' line " + std::to_string(lineNumber) + ": duplicate incompatible declaration for '" + path + "'");
			mDefinitions.push_back(std::move(definition));
		}
	}

	void Host::validateDefinition(const Definition& definition) const {
		std::filesystem::path path(definition.scriptPath);
		if (path.is_absolute() || path.has_root_name() || path.has_root_directory() || definition.scriptPath.find(':') != std::string::npos) {
			throw std::runtime_error("content '" + definition.contentFile + "' line " + std::to_string(definition.lineNumber) + ": absolute script paths are prohibited: '" + definition.scriptPath + "'");
		}
		for (const auto& component : path) {
			if (component == ".." || component == ".") throw std::runtime_error("content '" + definition.contentFile + "' line " + std::to_string(definition.lineNumber) + ": path traversal is prohibited: '" + definition.scriptPath + "'");
		}
		if (lower(path.extension().string()) != ".lua") throw std::runtime_error("content '" + definition.contentFile + "' line " + std::to_string(definition.lineNumber) + ": script path must end in .lua: '" + definition.scriptPath + "'");
		const bool global = containsFlag(definition, "GLOBAL");
		const bool menu = containsFlag(definition, "MENU");
		const bool load = containsFlag(definition, "LOAD");
		if ((global || menu || load) && definition.flags.size() != 1) throw std::runtime_error("content '" + definition.contentFile + "' line " + std::to_string(definition.lineNumber) + ": mutually incompatible flags for '" + definition.scriptPath + "'");
		std::filesystem::path resolved = std::filesystem::path(mVfsRoot) / path;
		if (!std::filesystem::is_regular_file(resolved)) throw std::runtime_error("content '" + definition.contentFile + "' line " + std::to_string(definition.lineNumber) + ": missing script '" + definition.scriptPath + "'");
	}

	void Host::startContainers() {
		for (const Definition& definition : mDefinitions) {
			if (containsFlag(definition, "MENU")) startInstance(definition, "MENU");
			if (containsFlag(definition, "GLOBAL")) startInstance(definition, "GLOBAL");
			if (containsFlag(definition, "PLAYER")) startInstance(definition, "PLAYER");
		}
	}

	void Host::startInstance(const Definition& definition, std::string_view container) {
		auto instance = std::make_unique<ScriptInstance>();
		instance->id = static_cast<std::uint32_t>(mInstances.size() + 1);
		instance->definitionOrder = definition.order;
		instance->lineNumber = definition.lineNumber;
		instance->contentFile = definition.contentFile;
		instance->scriptPath = definition.scriptPath;
		instance->container = std::string(container);
		mInstances.push_back(std::move(instance));
		ScriptInstance& script = *mInstances.back();
		createSandbox(script);
		std::filesystem::path path = std::filesystem::path(mVfsRoot) / definition.scriptPath;
		std::error_code error;
		auto size = std::filesystem::file_size(path, error);
		if (error || size > MaxScriptSize) throw std::runtime_error("content '" + definition.contentFile + "' script '" + definition.scriptPath + "' container " + std::string(container) + ": script is missing or exceeds 4 MiB");
		std::ifstream stream(path, std::ios::binary);
		std::string source(std::istreambuf_iterator<char>(stream), {});
		if (!source.empty() && static_cast<unsigned char>(source.front()) == 0x1b) throw std::runtime_error("content '" + definition.contentFile + "' script '" + definition.scriptPath + "' container " + std::string(container) + ": precompiled Lua bytecode is prohibited");
		if (luaL_loadbuffer(mLua, source.data(), source.size(), definition.scriptPath.c_str()) != 0) {
			std::string message = lua_tostring(mLua, -1); lua_pop(mLua, 1);
			throw std::runtime_error("content '" + definition.contentFile + "' script '" + definition.scriptPath + "' container " + std::string(container) + ": Lua load error: " + message);
		}
		lua_rawgeti(mLua, LUA_REGISTRYINDEX, script.environmentReference);
		lua_setfenv(mLua, -2);
		if (lua_pcall(mLua, 0, 1, 0) != 0) {
			std::string message = lua_tostring(mLua, -1); lua_pop(mLua, 1);
			throw std::runtime_error("content '" + definition.contentFile + "' script '" + definition.scriptPath + "' container " + std::string(container) + ": Lua runtime error: " + message);
		}
		if (!lua_istable(mLua, -1)) { lua_pop(mLua, 1); throw std::runtime_error("content '" + definition.contentFile + "' script '" + definition.scriptPath + "' container " + std::string(container) + ": script must return a table"); }
		registerScriptOutput(script);
		lua_pop(mLua, 1);
		lua_rawgeti(mLua, LUA_REGISTRYINDEX, script.environmentReference);
		lua_getfield(mLua, -1, "mwse");
		script.mwseGlobalVisible = !lua_isnil(mLua, -1);
		lua_pop(mLua, 2);
		if (auto it = script.engineHandlers.find("onInit"); it != script.engineHandlers.end()) callHandler(script, it->second, "engineHandlers.onInit", std::nullopt, std::nullopt);
		log(LogSeverity::Info, "script", "started isolated script environment", &script);
	}

	void Host::createSandbox(ScriptInstance& instance) {
		lua_newtable(mLua);
		const int environment = lua_gettop(mLua);
		static const char* safeFunctions[] = { "assert", "error", "ipairs", "next", "pairs", "pcall", "select", "tonumber", "tostring", "type", "unpack", "xpcall", "rawequal", "rawget", "rawset", "setmetatable", "getmetatable" };
		for (const char* name : safeFunctions) { lua_getglobal(mLua, name); lua_setfield(mLua, environment, name); }
		lua_getglobal(mLua, "_VERSION"); lua_setfield(mLua, environment, "_VERSION");
		for (const char* name : { "coroutine", "math", "string", "table" }) { pushSafeLibraryClone(name); lua_setfield(mLua, environment, name); }
		lua_pushvalue(mLua, environment); lua_setfield(mLua, environment, "_G");
		lua_pushlightuserdata(mLua, this);
		lua_pushinteger(mLua, static_cast<lua_Integer>(instance.id));
		lua_pushcclosure(mLua, &requireThunk, 2);
		lua_setfield(mLua, environment, "require");
		lua_newtable(mLua);
		lua_pushboolean(mLua, 0); lua_setfield(mLua, -2, "__metatable");
		lua_setmetatable(mLua, environment);
		instance.environmentReference = luaL_ref(mLua, LUA_REGISTRYINDEX);
	}

	void Host::pushSafeLibraryClone(const char* name) {
		lua_getglobal(mLua, name);
		if (!lua_istable(mLua, -1)) throw std::runtime_error(std::string("safe Lua library is missing: ") + name);
		lua_newtable(mLua);
		const int destination = lua_gettop(mLua);
		lua_pushnil(mLua);
		while (lua_next(mLua, -3) != 0) { lua_pushvalue(mLua, -2); lua_pushvalue(mLua, -2); lua_settable(mLua, destination); lua_pop(mLua, 1); }
		lua_remove(mLua, -2);
	}

	void Host::registerScriptOutput(ScriptInstance& instance) {
		const int output = lua_gettop(mLua);
		lua_getfield(mLua, output, "interfaceName");
		if (lua_isstring(mLua, -1)) instance.interfaceName = lua_tostring(mLua, -1);
		else if (!lua_isnil(mLua, -1)) throw std::runtime_error("interfaceName must be a string");
		lua_pop(mLua, 1);
		lua_getfield(mLua, output, "interface");
		instance.hasInterface = lua_istable(mLua, -1);
		if (!instance.hasInterface && !lua_isnil(mLua, -1)) throw std::runtime_error("interface must be a table");
		lua_pop(mLua, 1);
		if (instance.interfaceName.empty() == instance.hasInterface) throw std::runtime_error("interfaceName and interface must be declared together");

		auto collectHandlers = [&](const char* field, std::unordered_map<std::string, int>& destination) {
			lua_getfield(mLua, output, field);
			if (lua_isnil(mLua, -1)) { lua_pop(mLua, 1); return; }
			if (!lua_istable(mLua, -1)) throw std::runtime_error(std::string(field) + " must be a table");
			lua_pushnil(mLua);
			while (lua_next(mLua, -2) != 0) {
				if (!lua_isstring(mLua, -2) || !lua_isfunction(mLua, -1)) throw std::runtime_error(std::string(field) + " must map string names to functions");
				std::string name = lua_tostring(mLua, -2);
				lua_pushvalue(mLua, -1);
				destination[name] = luaL_ref(mLua, LUA_REGISTRYINDEX);
				lua_pop(mLua, 1);
			}
			lua_pop(mLua, 1);
		};
		collectHandlers("engineHandlers", instance.engineHandlers);
		collectHandlers("eventHandlers", instance.eventHandlers);
	}

	int Host::requireThunk(lua_State* state) {
		auto* self = static_cast<Host*>(lua_touserdata(state, lua_upvalueindex(1)));
		std::uint32_t id = static_cast<std::uint32_t>(lua_tointeger(state, lua_upvalueindex(2)));
		if (self == nullptr || !lua_isstring(state, 1)) return luaL_error(state, "require expects a module name string");
		ScriptInstance* instance = nullptr;
		for (const auto& item : self->mInstances) if (item->id == id) { instance = item.get(); break; }
		if (instance == nullptr) return luaL_error(state, "invalid script environment handle");
		std::string name = lua_tostring(state, 1);
		try {
			if (auto it = instance->loadedModules.find(name); it != instance->loadedModules.end()) { lua_rawgeti(state, LUA_REGISTRYINDEX, it->second); return 1; }
			if (name == "coroutine" || name == "math" || name == "string" || name == "table") self->pushSafeLibraryClone(name.c_str());
			else if (name == "openmw.compatibility") self->pushBuiltinPackage(name);
			else { int reference = self->loadSourceModule(*instance, name); lua_rawgeti(state, LUA_REGISTRYINDEX, reference); return 1; }
			lua_pushvalue(state, -1);
			instance->loadedModules[name] = luaL_ref(state, LUA_REGISTRYINDEX);
			return 1;
		}
		catch (const std::exception& error) { return luaL_error(state, "%s", error.what()); }
	}

	void Host::pushBuiltinPackage(std::string_view) {
		lua_newtable(mLua);
		lua_pushinteger(mLua, OpenMWApiRevision); lua_setfield(mLua, -2, "apiRevision");
		lua_pushinteger(mLua, BridgeVersion); lua_setfield(mLua, -2, "bridgeVersion");
		lua_pushboolean(mLua, 0); lua_setfield(mLua, -2, "gameplayBindingsAvailable");
		lua_pushcfunction(mLua, &unsupportedThunk); lua_setfield(mLua, -2, "requireGameplayBinding");
	}

	int Host::unsupportedThunk(lua_State* state) {
		return luaL_error(state, "unsupported capability: OpenMW gameplay packages begin in Milestone 4");
	}

	std::filesystem::path Host::resolveModulePath(std::string_view moduleName) const {
		if (moduleName.empty() || moduleName.size() > 256 || moduleName.find(".dll") != std::string_view::npos || moduleName.find('/') != std::string_view::npos || moduleName.find('\\') != std::string_view::npos || moduleName.find(':') != std::string_view::npos) throw std::runtime_error("native DLL modules and non-canonical module names are prohibited: " + std::string(moduleName));
		std::string relative(moduleName); std::replace(relative.begin(), relative.end(), '.', '/');
		for (std::string suffix : { ".lua", "/init.lua" }) {
			for (const std::string* root : { &mAuxiliaryRoot, &mVfsRoot }) {
				if (root->empty()) continue;
				std::filesystem::path candidate = std::filesystem::path(*root) / (relative + suffix);
				if (std::filesystem::is_regular_file(candidate)) return candidate;
			}
		}
		throw std::runtime_error("module not found in compatibility VFS: " + std::string(moduleName));
	}

	int Host::loadSourceModule(ScriptInstance& instance, std::string_view moduleName) {
		auto path = resolveModulePath(moduleName);
		std::error_code error; auto size = std::filesystem::file_size(path, error);
		if (error || size > MaxScriptSize) throw std::runtime_error("module is missing or exceeds 4 MiB: " + path.string());
		std::ifstream stream(path, std::ios::binary); std::string source(std::istreambuf_iterator<char>(stream), {});
		if (!source.empty() && static_cast<unsigned char>(source.front()) == 0x1b) throw std::runtime_error("precompiled Lua bytecode modules are prohibited: " + path.string());
		if (luaL_loadbuffer(mLua, source.data(), source.size(), path.string().c_str()) != 0) { std::string message = lua_tostring(mLua, -1); lua_pop(mLua, 1); throw std::runtime_error("Lua module load error: " + message); }
		lua_rawgeti(mLua, LUA_REGISTRYINDEX, instance.environmentReference); lua_setfenv(mLua, -2);
		if (lua_pcall(mLua, 0, 1, 0) != 0) { std::string message = lua_tostring(mLua, -1); lua_pop(mLua, 1); throw std::runtime_error("Lua module runtime error: " + message); }
		if (lua_isnil(mLua, -1)) { lua_pop(mLua, 1); lua_pushboolean(mLua, 1); }
		lua_pushvalue(mLua, -1); int reference = luaL_ref(mLua, LUA_REGISTRYINDEX); lua_pop(mLua, 1);
		instance.loadedModules[std::string(moduleName)] = reference;
		return reference;
	}

	Status Host::update(const FrameUpdate* updateData) {
		if (updateData == nullptr) return Status::InvalidArgument;
		if (updateData->structureSize != sizeof(FrameUpdate)) return Status::StructureSizeMismatch;
		if (updateData->abiVersion != BridgeAbiVersion) return Status::AbiMismatch;
		if (mState != LifecycleState::Running) return Status::InvalidState;
		mFrameNumber = updateData->frameNumber;
		deliverDelayedEvents();
		callEngineHandlers("onUpdate", updateData->realDeltaSeconds);
		mPendingEvents.swap(mNextEvents); mNextEvents.clear();
		writeReportArtifacts();
		return Status::Ok;
	}

	Status Host::queueEvent(const QueuedEvent* eventData) {
		if (eventData == nullptr) return Status::InvalidArgument;
		if (eventData->structureSize != sizeof(QueuedEvent)) return Status::StructureSizeMismatch;
		if (eventData->abiVersion != BridgeAbiVersion) return Status::AbiMismatch;
		if (mState != LifecycleState::Running) return Status::InvalidState;
		try {
			std::string name = copyBridgeString(eventData->name, "event.name", false);
			std::string payload = copyBridgeString(eventData->serializedPayload, "event.payload", true);
			mNextEvents.push_back({ mNextEventSequence++, std::move(name), std::move(payload) });
			return Status::Ok;
		}
		catch (...) { return Status::InvalidArgument; }
	}

	void Host::deliverDelayedEvents() {
		for (const auto& eventData : mPendingEvents) { if (mDelayedDeliveries.size() < 128) mDelayedDeliveries.push_back(std::to_string(eventData.sequence) + ":" + eventData.name); callEventHandlers(eventData); }
		mPendingEvents.clear();
	}

	void Host::callEngineHandlers(std::string_view name, double argument) {
		for (auto& item : mInstances) if (auto it = item->engineHandlers.find(std::string(name)); it != item->engineHandlers.end()) { if (mEngineHandlerOrder.size() < 128) mEngineHandlerOrder.push_back(item->container + ":" + item->scriptPath + ":" + std::string(name)); callHandler(*item, it->second, "engineHandlers." + std::string(name), argument, std::nullopt); }
	}

	void Host::callEventHandlers(const DelayedEvent& eventData) {
		for (std::size_t i = mInstances.size(); i > 0; --i) { auto& item = *mInstances[i - 1]; if (auto it = item.eventHandlers.find(eventData.name); it != item.eventHandlers.end()) { if (mEventHandlerOrder.size() < 128) mEventHandlerOrder.push_back(item.container + ":" + item.scriptPath + ":" + eventData.name); callHandler(item, it->second, "eventHandlers." + eventData.name, std::nullopt, eventData.payload); } }
	}

	bool Host::callHandler(ScriptInstance& instance, int functionReference, std::string_view diagnosticName, std::optional<double> numberArgument, std::optional<std::string_view> stringArgument) {
		lua_rawgeti(mLua, LUA_REGISTRYINDEX, functionReference); int arguments = 0;
		if (numberArgument) { lua_pushnumber(mLua, *numberArgument); ++arguments; }
		if (stringArgument) { lua_pushlstring(mLua, stringArgument->data(), stringArgument->size()); ++arguments; }
		if (lua_pcall(mLua, arguments, 0, 0) == 0) return true;
		std::string message = lua_tostring(mLua, -1); lua_pop(mLua, 1);
		std::string diagnostic = instance.contentFile + "|" + instance.container + "|" + instance.scriptPath + "|" + std::string(diagnosticName) + "|" + message;
		if (mDiagnostics.size() < 128) mDiagnostics.push_back(diagnostic); log(LogSeverity::Error, "handler", diagnostic, &instance); return false;
	}

	Status Host::reload() {
		if (mState != LifecycleState::Running) return Status::InvalidState;
		log(LogSeverity::Info, "reload", "explicit runtime reload requested");
		destroyRuntime(); ++mReloadCount;
		Status result = initializeRuntime();
		writeReportArtifacts(); return result;
	}

	LifecycleState Host::getLifecycleState() const { return mState; }

	void Host::log(LogSeverity severity, std::string_view category, std::string_view message, const ScriptInstance* instance) {
		std::string_view content = instance ? std::string_view(instance->contentFile) : std::string_view(mContentFile);
		std::string_view script = instance ? std::string_view(instance->scriptPath) : std::string_view();
		std::string_view container = instance ? std::string_view(instance->container) : std::string_view();
		LogMessage bridgeMessage{ sizeof(LogMessage), BridgeAbiVersion, severity, 0, asBridgeString(category), asBridgeString(content), asBridgeString(script), asBridgeString(container), asBridgeString(message) };
		if (mCallbacks.log != nullptr) mCallbacks.log(mCallbacks.logUserData, &bridgeMessage);
		if (!mReportDirectory.empty()) {
			try { std::filesystem::create_directories(mReportDirectory); std::ofstream stream(std::filesystem::path(mReportDirectory) / "openmw-host-events.jsonl", std::ios::binary | std::ios::app); stream << "{\"severity\":" << static_cast<std::uint32_t>(severity) << ",\"category\":\"" << jsonEscape(category) << "\",\"contentFile\":\"" << jsonEscape(content) << "\",\"scriptPath\":\"" << jsonEscape(script) << "\",\"container\":\"" << jsonEscape(container) << "\",\"message\":\"" << jsonEscape(message) << "\"}\n"; } catch (...) {}
		}
	}

	std::string Host::buildContainerReport() const {
		std::size_t menu = 0, global = 0, player = 0; for (const auto& d : mDefinitions) { menu += containsFlag(d, "MENU"); global += containsFlag(d, "GLOBAL"); player += containsFlag(d, "PLAYER"); }
		std::ostringstream stream; stream << "{\"contentFile\":\"" << jsonEscape(mContentFile) << "\",\"definitionCount\":" << mDefinitions.size() << ",\"menuDefinitions\":" << menu << ",\"globalDefinitions\":" << global << ",\"playerDefinitions\":" << player << ",\"instances\":[";
		for (std::size_t i=0;i<mInstances.size();++i) { if(i)stream<<','; const auto& s=*mInstances[i]; stream << "{\"id\":" << s.id << ",\"environmentId\":\"generation-" << mRuntimeGeneration << "-environment-" << s.id << "\",\"contentFile\":\"" << jsonEscape(s.contentFile) << "\",\"scriptPath\":\"" << jsonEscape(s.scriptPath) << "\",\"container\":\"" << s.container << "\",\"lineNumber\":" << s.lineNumber << ",\"interfaceName\":\"" << jsonEscape(s.interfaceName) << "\",\"hasInterface\":" << (s.hasInterface?"true":"false") << ",\"mwseGlobalVisible\":" << (s.mwseGlobalVisible?"true":"false") << '}'; }
		stream << "]}"; return stream.str();
	}

	std::string Host::buildHandlerReport() const { std::ostringstream s; s << "{\"engineHandlerOrder\":" << jsonStringArray(mEngineHandlerOrder) << ",\"eventHandlerOrder\":" << jsonStringArray(mEventHandlerOrder) << ",\"delayedDeliveries\":" << jsonStringArray(mDelayedDeliveries) << ",\"diagnostics\":" << jsonStringArray(mDiagnostics) << '}'; return s.str(); }
	std::string Host::buildBridgeReport() const { std::ostringstream s; s << "{\"abiAccepted\":true,\"abiVersion\":" << BridgeAbiVersion << ",\"hostApiStructureSize\":" << sizeof(HostApi) << ",\"initializationStructureSize\":" << sizeof(InitializationConfig) << ",\"runtimeVersion\":\"" << LUAJIT_VERSION << "\",\"apiRevision\":" << OpenMWApiRevision << ",\"bridgeVersion\":" << BridgeVersion << ",\"capabilities\":" << (CapabilityIsolatedLuaState|CapabilitySandboxedSourceModules|CapabilityMenuContainer|CapabilityGlobalContainer|CapabilityPlayerContainer|CapabilityDelayedEvents|CapabilityReload) << ",\"runtimeGeneration\":" << mRuntimeGeneration << ",\"runtimeOwnedAllocator\":true,\"importsMwseLua\":false,\"allocatedBytes\":" << mAllocatedBytes << '}'; return s.str(); }
	std::string Host::buildReloadReport() const { std::ostringstream s; s << "{\"reloadCount\":" << mReloadCount << ",\"runtimeGeneration\":" << mRuntimeGeneration << ",\"state\":" << static_cast<std::uint32_t>(mState) << ",\"cleanShutdown\":" << (mState==LifecycleState::Stopped?"true":"false") << '}'; return s.str(); }
	std::string Host::buildReport() const { std::ostringstream s; s << "{\"schemaVersion\":1,\"state\":" << static_cast<std::uint32_t>(mState) << ",\"lastError\":\"" << jsonEscape(mLastError) << "\",\"bridge\":" << buildBridgeReport() << ",\"containers\":" << buildContainerReport() << ",\"handlers\":" << buildHandlerReport() << ",\"reload\":" << buildReloadReport() << '}'; return s.str(); }

	void Host::writeReportArtifacts() {
		if (mReportDirectory.empty()) return;
		try { auto root=std::filesystem::path(mReportDirectory); writeAtomic(root/"bridge-runtime-report.json", buildBridgeReport()); writeAtomic(root/"parsed-container-report.json", buildContainerReport()); writeAtomic(root/"handler-order-report.json", buildHandlerReport()); writeAtomic(root/"reload-shutdown-report.json", buildReloadReport()); writeAtomic(root/"openmw-host-report.json", buildReport()); } catch (const std::exception& error) { if (mCallbacks.log) { LogMessage message{sizeof(LogMessage),BridgeAbiVersion,LogSeverity::Error,0,asBridgeString("report"),asBridgeString(mContentFile),{}, {},asBridgeString(error.what())}; mCallbacks.log(mCallbacks.logUserData,&message); } }
	}

	Status Host::getReport(char* buffer, std::uint32_t capacity, std::uint32_t* requiredSize) {
		if (requiredSize == nullptr) return Status::InvalidArgument; std::string report=buildReport(); if(report.size()>=UINT32_MAX)return Status::BufferTooSmall; *requiredSize=static_cast<std::uint32_t>(report.size()+1); if(buffer==nullptr||capacity<*requiredSize)return Status::BufferTooSmall; std::memcpy(buffer,report.c_str(),report.size()+1); return Status::Ok;
	}

}
