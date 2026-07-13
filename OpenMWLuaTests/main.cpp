#include "../SharedSE/OpenMWLuaBridge.h"

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
#include <windows.h>

using namespace mwse::openmw;

namespace {
	std::vector<std::string> passedTests;

	void require(bool condition, std::string_view name) {
		if (!condition) throw std::runtime_error(std::string(name));
		passedTests.emplace_back(name);
	}

	void writeFile(const std::filesystem::path& path, std::string_view data) {
		std::filesystem::create_directories(path.parent_path());
		std::ofstream stream(path, std::ios::binary | std::ios::trunc);
		stream.write(data.data(), static_cast<std::streamsize>(data.size()));
		if (!stream) throw std::runtime_error("failed to write test fixture: " + path.string());
	}

	void __cdecl logCallback(void*, const LogMessage*) {}

	StringView view(const std::string& value) {
		return { value.data(), static_cast<std::uint32_t>(value.size()) };
	}

	InitializationConfig configFor(const std::filesystem::path& root, const std::string& scripts,
		const std::string& content, std::uint32_t flags, const std::filesystem::path& reports = {}) {
		static std::string rootString, reportString;
		rootString = root.string();
		reportString = reports.string();
		InitializationConfig config{};
		config.structureSize = sizeof(config);
		config.abiVersion = BridgeAbiVersion;
		config.flags = flags;
		config.callbacks = { sizeof(BridgeCallbacks), BridgeAbiVersion, &logCallback, nullptr };
		config.vfsRoot = view(rootString);
		config.scriptsFile = view(scripts);
		config.contentFile = view(content);
		config.reportDirectory = view(reportString);
		return config;
	}

	std::string report(const HostApi& api) {
		std::uint32_t requiredSize = 0;
		require(api.getReport(nullptr, 0, &requiredSize) == Status::BufferTooSmall, "report-size-negotiation");
		std::string value(requiredSize, '\0');
		require(api.getReport(value.data(), requiredSize, &requiredSize) == Status::Ok, "report-copy");
		if (!value.empty() && value.back() == '\0') value.pop_back();
		return value;
	}

	void expectParseFailure(const HostApi& api, const std::filesystem::path& root, std::string_view declaration,
		std::string_view testName, bool createReferencedScript = true) {
		writeFile(root / "case.omwscripts", declaration);
		if (createReferencedScript) writeFile(root / "ok.lua", "return {}\n");
		std::string scripts = "case.omwscripts", content = "case.omwscripts";
		auto config = configFor(root, scripts, content, InitializationEnabled | InitializationParseOnly);
		require(api.initialize(&config) != Status::Ok, testName);
		api.shutdown();
	}
}

int main(int argc, char** argv) {
	std::filesystem::path temporary;
	try {
		std::filesystem::path dllPath = argc > 1 ? argv[1] : std::filesystem::path("openmw-lua.dll");
		HMODULE module = LoadLibraryW(std::filesystem::absolute(dllPath).wstring().c_str());
		require(module != nullptr, "support-dll-load");
		auto query = reinterpret_cast<QueryApiFunction>(GetProcAddress(module, "OpenMWLua_QueryApi"));
		require(query != nullptr, "bridge-export");
		HostApi api{};
		require(query(BridgeAbiVersion + 1, sizeof(api), &api) == Status::AbiMismatch, "bridge-abi-mismatch");
		require(query(BridgeAbiVersion, sizeof(api) - 1, &api) == Status::StructureSizeMismatch, "bridge-size-mismatch");
		require(query(BridgeAbiVersion, sizeof(api), &api) == Status::Ok, "bridge-negotiation");
		require(api.structureSize == sizeof(HostApi) && api.abiVersion == BridgeAbiVersion, "bridge-layout-accepted");

		InitializationConfig missingCallbacks{};
		missingCallbacks.structureSize = sizeof(missingCallbacks);
		missingCallbacks.abiVersion = BridgeAbiVersion;
		missingCallbacks.callbacks = { sizeof(BridgeCallbacks), BridgeAbiVersion, nullptr, nullptr };
		require(api.initialize(&missingCallbacks) == Status::MissingCallback, "missing-callback-rejected");

		temporary = std::filesystem::temp_directory_path() / ("openmw-lua-tests-" + std::to_string(GetCurrentProcessId()));
		std::filesystem::remove_all(temporary);
		std::filesystem::create_directories(temporary);

		writeFile(temporary / "modules/helper.lua", "return { value = 42 }\n");
		writeFile(temporary / "menu.lua", R"(
local helper = require('modules.helper')
if rawget(_G, 'environmentSentinel') ~= nil then error('sandbox leaked') end
environmentSentinel = 'menu'
return { interfaceName='MenuInterface', interface={value=helper.value},
  engineHandlers={onUpdate=function(dt) assert(type(dt)=='number') end},
  eventHandlers={Milestone3Event=function(data) assert(type(data)=='string') end} }
)");
		writeFile(temporary / "global.lua", R"(
if rawget(_G, 'environmentSentinel') ~= nil then error('sandbox leaked') end
environmentSentinel = 'global'
return { interfaceName='GlobalInterface', interface={},
  engineHandlers={onUpdate=function(dt) end},
  eventHandlers={Milestone3Event=function(data) error('intentional handler failure') end} }
)");
		writeFile(temporary / "player.lua", R"(
if rawget(_G, 'environmentSentinel') ~= nil then error('sandbox leaked') end
environmentSentinel = 'player'
return { interfaceName='PlayerInterface', interface={},
  engineHandlers={onUpdate=function(dt) end},
  eventHandlers={Milestone3Event=function(data) end} }
)");
		writeFile(temporary / "synthetic.omwscripts", "# ordering fixture\n MENU : menu.lua\nGLOBAL: global.lua\nPLAYER, CUSTOM: player.lua\n");
		std::string scripts = "synthetic.omwscripts", content = "synthetic.omwscripts";
		auto config = configFor(temporary, scripts, content, InitializationEnabled | InitializationHarnessMode, temporary / "reports");
		require(api.initialize(&config) == Status::Ok, "runtime-initialize");
		require(api.getLifecycleState() == LifecycleState::Running, "runtime-running");
		FrameUpdate frame{ sizeof(FrameUpdate), BridgeAbiVersion, 1, 0.016, 1.0, 0.0, 0, 0 };
		require(api.update(&frame) == Status::Ok, "first-frame-update");
		frame.frameNumber = 2;
		require(api.update(&frame) == Status::Ok, "second-frame-update");
		std::string runtimeReport = report(api);
		require(runtimeReport.find("\"menuDefinitions\":1") != std::string::npos, "parser-menu-count");
		require(runtimeReport.find("\"globalDefinitions\":1") != std::string::npos, "parser-global-count");
		require(runtimeReport.find("\"playerDefinitions\":1") != std::string::npos, "parser-player-count");
		require(runtimeReport.find("generation-1-environment-1") != std::string::npos
			&& runtimeReport.find("generation-1-environment-2") != std::string::npos
			&& runtimeReport.find("generation-1-environment-3") != std::string::npos, "sandbox-distinct-environments");
		require(runtimeReport.find("\"mwseGlobalVisible\":true") == std::string::npos, "sandbox-no-mwse-global");
		require(runtimeReport.find("MenuInterface") != std::string::npos && runtimeReport.find("PlayerInterface") != std::string::npos, "interfaces-registered");
		auto engineMenu = runtimeReport.find("MENU:menu.lua:onUpdate");
		auto engineGlobal = runtimeReport.find("GLOBAL:global.lua:onUpdate");
		auto enginePlayer = runtimeReport.find("PLAYER:player.lua:onUpdate");
		require(engineMenu < engineGlobal && engineGlobal < enginePlayer, "engine-handler-direct-order");
		auto eventPlayer = runtimeReport.find("PLAYER:player.lua:Milestone3Event");
		auto eventGlobal = runtimeReport.find("GLOBAL:global.lua:Milestone3Event");
		auto eventMenu = runtimeReport.find("MENU:menu.lua:Milestone3Event");
		require(eventPlayer < eventGlobal && eventGlobal < eventMenu, "event-handler-reverse-order");
		require(runtimeReport.find("intentional handler failure") != std::string::npos && eventMenu != std::string::npos, "handler-failure-isolation");
		require(runtimeReport.find("1:Milestone3Event") != std::string::npos, "delayed-event-delivery");

		require(api.reload() == Status::Ok, "explicit-reload");
		frame.frameNumber = 3; api.update(&frame); frame.frameNumber = 4; api.update(&frame);
		std::string reloadReport = report(api);
		require(reloadReport.find("\"reloadCount\":1") != std::string::npos
			&& reloadReport.find("\"runtimeGeneration\":2") != std::string::npos, "reload-recreated-runtime");
		require(api.shutdown() == Status::Ok && api.getLifecycleState() == LifecycleState::Stopped, "clean-shutdown");

		writeFile(temporary / "multi.lua", "return {}\n");
		writeFile(temporary / "valid.omwscripts", "# comment\nPLAYER, CUSTOM, NPC : multi.lua\n");
		scripts = "valid.omwscripts"; content = scripts;
		auto parseConfig = configFor(temporary, scripts, content, InitializationEnabled | InitializationParseOnly);
		require(api.initialize(&parseConfig) == Status::Ok, "parser-comments-multiple-flags"); api.shutdown();
		expectParseFailure(api, temporary, "UNKNOWN: ok.lua\n", "parser-unknown-flag");
		expectParseFailure(api, temporary, "GLOBAL ok.lua\n", "parser-malformed-declaration");
		expectParseFailure(api, temporary, "GLOBAL:\n", "parser-missing-path");
		expectParseFailure(api, temporary, "GLOBAL: missing.lua\n", "parser-missing-script", false);
		expectParseFailure(api, temporary, "GLOBAL: ../ok.lua\n", "parser-path-traversal");
		expectParseFailure(api, temporary, "GLOBAL: C:/ok.lua\n", "parser-absolute-path");
		expectParseFailure(api, temporary, "GLOBAL,PLAYER: ok.lua\n", "parser-incompatible-flags");
		expectParseFailure(api, temporary, "GLOBAL: ok.lua\nPLAYER: ok.lua\n", "parser-duplicate-incompatible");

		writeFile(temporary / "not_table.lua", "return 12\n");
		writeFile(temporary / "not_table.omwscripts", "GLOBAL: not_table.lua\n");
		scripts = "not_table.omwscripts"; content = scripts; auto notTable = configFor(temporary, scripts, content, InitializationEnabled);
		require(api.initialize(&notTable) != Status::Ok, "returned-table-validation"); api.shutdown();
		writeFile(temporary / "native.lua", "require('native.dll')\nreturn {}\n");
		writeFile(temporary / "native.omwscripts", "GLOBAL: native.lua\n");
		scripts = "native.omwscripts"; content = scripts; auto nativeConfig = configFor(temporary, scripts, content, InitializationEnabled);
		require(api.initialize(&nativeConfig) != Status::Ok, "native-dll-module-rejected"); api.shutdown();
		writeFile(temporary / "bytecode.lua", std::string("\x1bLua", 4));
		writeFile(temporary / "bytecode.omwscripts", "GLOBAL: bytecode.lua\n");
		scripts = "bytecode.omwscripts"; content = scripts; auto bytecodeConfig = configFor(temporary, scripts, content, InitializationEnabled);
		require(api.initialize(&bytecodeConfig) != Status::Ok, "precompiled-bytecode-rejected"); api.shutdown();

		if (argc > 2) {
			std::filesystem::path ncgPath = argv[2];
			std::string ncgScripts = ncgPath.filename().string();
			std::string ncgContent = ncgScripts;
			auto ncgConfig = configFor(ncgPath.parent_path(), ncgScripts, ncgContent, InitializationEnabled | InitializationParseOnly);
			require(api.initialize(&ncgConfig) == Status::Ok, "ncg-parser-success");
			std::string ncgReport = report(api);
			require(ncgReport.find("\"menuDefinitions\":1") != std::string::npos
				&& ncgReport.find("\"globalDefinitions\":1") != std::string::npos
				&& ncgReport.find("\"playerDefinitions\":1") != std::string::npos, "ncg-exact-container-counts");
			api.shutdown();
		}

		std::filesystem::remove_all(temporary);
		std::cout << "{\"passed\":true,\"tests\":[";
		for (std::size_t i=0;i<passedTests.size();++i) { if(i)std::cout << ','; std::cout << '\"' << passedTests[i] << '\"'; }
		std::cout << "]}\n";
		return 0;
	}
	catch (const std::exception& error) {
		if (!temporary.empty()) std::filesystem::remove_all(temporary);
		std::cerr << "{\"passed\":false,\"error\":\"" << error.what() << "\"}\n";
		return 1;
	}
}
