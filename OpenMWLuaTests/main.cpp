#include "../SharedSE/OpenMWLuaBridge.h"

#include <algorithm>
#include <cstring>
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
	Status __cdecl gameSettingCallback(void*, StringView name, BridgeValue* value) {
		if(value==nullptr||value->structureSize!=sizeof(BridgeValue)||value->abiVersion!=BridgeAbiVersion)return Status::InvalidArgument;
		const std::string key(name.data,name.size);value->string={};value->number=0.0;
		if(key=="sHealth"){static constexpr char Health[]="Health";value->type=ValueType::String;value->string={Health,sizeof(Health)-1};}
		else if(key=="iLevelupMajorMult"){value->type=ValueType::Number;value->number=0.0;}else value->type=ValueType::None;
		return Status::Ok;
	}
	std::uint32_t __cdecl contentFileCountCallback(void*) { return 2; }
	Status __cdecl contentFileCallback(void*, std::uint32_t index, StringView* value) {
		static constexpr char Files[][16]={"Morrowind.esm","ncg.omwaddon"};if(value==nullptr||index>=2)return Status::InvalidArgument;*value={Files[index],static_cast<std::uint32_t>(std::strlen(Files[index]))};return Status::Ok;
	}

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
		config.callbacks = { sizeof(BridgeCallbacks), BridgeAbiVersion, &logCallback, nullptr,
			&gameSettingCallback, &contentFileCountCallback, &contentFileCallback, nullptr };
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
		missingCallbacks.callbacks = { sizeof(BridgeCallbacks), BridgeAbiVersion, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr };
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
		FrameUpdate frame{ sizeof(FrameUpdate), BridgeAbiVersion, 1, 0.016, 1.0, 1.0, 1.0, 30.0, 0, 0 };
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

		writeFile(temporary / "foundation_provider.lua", R"(
local async=require('openmw.async')
local compat=require('openmw.compatibility')
local core=require('openmw.core')
local storage=require('openmw.storage')
local section=storage.globalSection('FoundationGlobal')
local original={value=1}
section:set('copy',original)
original.value=99
assert(section:getCopy('copy').value==1)
section:subscribe(async:callback(function(name,key) compat.recordFoundationProbe('storage-global-subscription',name=='FoundationGlobal' and key=='changed') end))
section:set('changed',7)
local registered=async:registerTimerCallback('registered',function(value) compat.recordFoundationProbe('async-registered-simulation',value==9) end)
async:newSimulationTimer(0,registered,9)
async:newUnsavableGameTimer(0,function() compat.recordFoundationProbe('async-unsavable-game',true) end)
core.sendGlobalEvent('FoundationGlobalEvent',{value=7})
return {interfaceName='FoundationInterface',interface={answer=42}}
)");
		writeFile(temporary / "foundation_consumer.lua", R"(
local async=require('openmw.async')
local compat=require('openmw.compatibility')
local core=require('openmw.core')
local I=require('openmw.interfaces')
local storage=require('openmw.storage')
local util=require('openmw.util')
assert(core.API_REVISION==70 and core.getGMST('sHealth')=='Health')
assert(core.contentFiles.has('mOrRoWiNd.EsM') and core.contentFiles.indexOf('ncg.omwaddon')==2)
local v=util.vector2(3,4)
local normalized,length=v:normalize()
local color=util.color.rgb(.8,.3,.4)
assert(v:length()==5 and v:length2()==25 and (v*2).x==6 and normalized:length()>0.999 and length==5 and color.a==1)
local mutable=pcall(function() v.x=10 end)
compat.recordFoundationProbe('util-vector-color',not mutable and util.round(-1.5)==-2)
assert(I.FoundationInterface.answer==42)
local writable=pcall(function() I.FoundationInterface.answer=0 end)
compat.recordFoundationProbe('interfaces-lookup-readonly',not writable)
assert(storage.globalSection('FoundationGlobal'):get('changed')==7)
local callback=async:callback(function(value) return value+1 end)
compat.recordFoundationProbe('async-callback-callable',callback(4)==5)
local selfAvailable=pcall(function() return require('openmw.self') end)
compat.recordFoundationProbe('self-context-rejected-global',not selfAvailable)
compat.recordFoundationProbe('core-time-content-gmst',type(core.getSimulationTime())=='number' and core.contentFiles.list[1]=='morrowind.esm')
return {eventHandlers={FoundationGlobalEvent=function(data) compat.recordFoundationProbe('core-delayed-global-event',data.value==7) end}}
)");
		writeFile(temporary / "foundation_player.lua", R"(
local async=require('openmw.async')
local compat=require('openmw.compatibility')
local self=require('openmw.self')
local storage=require('openmw.storage')
assert(self._mwseFoundationAvailable==true)
local section=storage.playerSection('FoundationPlayer')
section:subscribe(async:callback(function(name,key) compat.recordFoundationProbe('storage-player-subscription',name=='FoundationPlayer' and key=='value') end))
section:set('value',11)
local globalWritable=pcall(function() storage.globalSection('FoundationGlobal'):set('bad',1) end)
compat.recordFoundationProbe('storage-context-permissions',not globalWritable and section:get('value')==11)
compat.recordFoundationProbe('self-player-context',true)
return {}
)");
		writeFile(temporary / "foundation_menu.lua", R"(
local compat=require('openmw.compatibility')
local storage=require('openmw.storage')
local selfAvailable=pcall(function() return require('openmw.self') end)
storage.playerSection('FoundationMenu'):set('value',3)
compat.recordFoundationProbe('self-context-rejected-menu',not selfAvailable)
compat.recordFoundationProbe('storage-menu-player-scope',storage.playerSection('FoundationMenu'):get('value')==3)
return {}
)");
		writeFile(temporary / "foundation.omwscripts", "GLOBAL: foundation_provider.lua\nGLOBAL: foundation_consumer.lua\nPLAYER: foundation_player.lua\nMENU: foundation_menu.lua\n");
		scripts="foundation.omwscripts";content=scripts;auto foundationConfig=configFor(temporary,scripts,content,InitializationEnabled|InitializationHarnessMode,temporary/"foundation-reports");
		require(api.initialize(&foundationConfig)==Status::Ok,"foundation-runtime-initialize");
		frame={sizeof(FrameUpdate),BridgeAbiVersion,1,0.016,1.0,1.0,1.0,30.0,0,0};require(api.update(&frame)==Status::Ok,"foundation-first-update");frame.frameNumber=2;frame.simulationTimeSeconds=2.0;require(api.update(&frame)==Status::Ok,"foundation-second-update");
		const std::string foundationReport=report(api);
		for(const char* probe:{"util-vector-color","interfaces-lookup-readonly","async-registered-simulation","async-unsavable-game","async-callback-callable","storage-global-subscription","storage-player-subscription","storage-context-permissions","storage-menu-player-scope","core-time-content-gmst","core-delayed-global-event","self-player-context","self-context-rejected-global","self-context-rejected-menu"})require(foundationReport.find(std::string("\"")+probe+"\":true")!=std::string::npos,std::string("foundation-probe-")+probe);
		require(foundationReport.find("\"fired\":2")!=std::string::npos,"foundation-timer-count");
		require(foundationReport.find("\"globalSections\":1")!=std::string::npos&&foundationReport.find("\"playerSections\":2")!=std::string::npos,"foundation-storage-scope-counts");
		require(api.shutdown()==Status::Ok,"foundation-clean-shutdown");

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
