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
	struct GameplayFixture {
		std::uint32_t generation = 1;
		double attributes[8] = {40,41,42,43,44,45,46,47};
		double attributeModifier[8] = {};
		double attributeDamage[8] = {};
		double skills[27] = {};
		double skillModifier[27] = {};
		double skillDamage[27] = {};
		double skillProgress[27] = {};
		double healthBase = 100;
		double healthCurrent = 87;
		double healthModifier = 0;
		double level = 3;
		double levelProgress = 4;
		bool spellAdded = false;
	};
	GameplayFixture gameplay;
	const char* AttributeIds[] = {"strength","intelligence","willpower","agility","speed","endurance","personality","luck"};
	const char* SkillIds[] = {"block","armorer","mediumarmor","heavyarmor","bluntweapon","longblade","axe","spear","athletics","enchant","destruction","alteration","illusion","conjuration","mysticism","restoration","alchemy","unarmored","security","sneak","acrobatics","lightarmor","shortblade","marksman","mercantile","speechcraft","handtohand"};
	void textValue(BridgeText& output, std::string_view value) { output={};output.size=static_cast<std::uint32_t>(value.size());std::memcpy(output.data,value.data(),value.size()); }
	Status __cdecl validateGameplayHandle(void* userData, OpaqueHandle handle, HandleType expected) {
		auto* fixture=static_cast<GameplayFixture*>(userData);if(!fixture||handle.value==0)return Status::InvalidHandle;if(handle.reserved!=static_cast<std::uint32_t>(expected))return Status::WrongHandleType;if(handle.generation!=fixture->generation)return Status::StaleHandle;if((expected==HandleType::Player&&handle.value!=1)||(expected==HandleType::Cell&&handle.value!=2))return Status::InvalidHandle;return Status::Ok;
	}
	Status __cdecl playerCallback(void* userData,ObjectSnapshot* output){auto* fixture=static_cast<GameplayFixture*>(userData);if(!fixture||!output||output->structureSize!=sizeof(*output)||output->abiVersion!=BridgeAbiVersion)return Status::InvalidArgument;ObjectSnapshot result{};result.structureSize=sizeof(result);result.abiVersion=BridgeAbiVersion;result.object={1,fixture->generation,static_cast<std::uint32_t>(HandleType::Player)};result.cell={2,fixture->generation,static_cast<std::uint32_t>(HandleType::Cell)};result.typeMask=7;textValue(result.recordId,"player");*output=result;return Status::Ok;}
	Status __cdecl cellCallback(void* userData,OpaqueHandle handle,CellSnapshot* output){auto status=validateGameplayHandle(userData,handle,HandleType::Cell);if(status!=Status::Ok)return status;if(!output||output->structureSize!=sizeof(*output)||output->abiVersion!=BridgeAbiVersion)return Status::InvalidArgument;CellSnapshot result{};result.structureSize=sizeof(result);result.abiVersion=BridgeAbiVersion;result.handle=handle;result.flags=CellHasWater|CellHasSky|CellIsExterior|CellHasWaterLevel;result.gridX=1;result.gridY=-2;result.waterLevel=0;textValue(result.id,"#1 -2");textValue(result.name,"Balmora");textValue(result.displayName,"Balmora");textValue(result.region,"bitter coast region");textValue(result.worldSpaceId,"sys::default");*output=result;return Status::Ok;}
	Status __cdecl statCallback(void* userData,OpaqueHandle handle,StatKind kind,std::uint32_t index,StatSnapshot* output){auto status=validateGameplayHandle(userData,handle,HandleType::Player);if(status!=Status::Ok)return status;auto* fixture=static_cast<GameplayFixture*>(userData);if(!output||output->structureSize!=sizeof(*output)||output->abiVersion!=BridgeAbiVersion)return Status::InvalidArgument;StatSnapshot result{};result.structureSize=sizeof(result);result.abiVersion=BridgeAbiVersion;if(kind==StatKind::Attribute){if(index>=8)return Status::OutOfRange;result.base=fixture->attributes[index];result.modifier=fixture->attributeModifier[index];result.damage=fixture->attributeDamage[index];result.current=result.modified=result.base-result.damage+result.modifier;result.writableMask=25;}else if(kind==StatKind::Skill){if(index>=27)return Status::OutOfRange;result.base=fixture->skills[index];result.modifier=fixture->skillModifier[index];result.damage=fixture->skillDamage[index];result.current=result.modified=result.base-result.damage+result.modifier;result.progress=fixture->skillProgress[index];result.writableMask=57;}else if(kind==StatKind::Health){result.base=fixture->healthBase;result.current=fixture->healthCurrent;result.modified=result.base+fixture->healthModifier;result.modifier=fixture->healthModifier;result.writableMask=11;}else if(kind==StatKind::Level){result.base=result.current=result.modified=fixture->level;result.progress=fixture->levelProgress;result.writableMask=34;}else return Status::Unsupported;*output=result;return Status::Ok;}
	Status __cdecl setStatCallback(void* userData,OpaqueHandle handle,StatKind kind,std::uint32_t index,StatField field,double value){auto status=validateGameplayHandle(userData,handle,HandleType::Player);if(status!=Status::Ok)return status;auto* f=static_cast<GameplayFixture*>(userData);if(kind==StatKind::Attribute&&index<8&&field==StatField::Base)f->attributes[index]=value;else if(kind==StatKind::Attribute&&index<8&&field==StatField::Modifier)f->attributeModifier[index]=value;else if(kind==StatKind::Attribute&&index<8&&field==StatField::Damage)f->attributeDamage[index]=value;else if(kind==StatKind::Skill&&index<27&&field==StatField::Base)f->skills[index]=value;else if(kind==StatKind::Skill&&index<27&&field==StatField::Modifier)f->skillModifier[index]=value;else if(kind==StatKind::Skill&&index<27&&field==StatField::Damage)f->skillDamage[index]=value;else if(kind==StatKind::Skill&&index<27&&field==StatField::Progress)f->skillProgress[index]=value;else if(kind==StatKind::Health&&field==StatField::Base)f->healthBase=value;else if(kind==StatKind::Health&&field==StatField::Current)f->healthCurrent=value;else if(kind==StatKind::Health&&field==StatField::Modifier)f->healthModifier=value;else if(kind==StatKind::Level&&field==StatField::Current)f->level=value;else if(kind==StatKind::Level&&field==StatField::Progress)f->levelProgress=value;else return Status::ReadOnly;return Status::Ok;}
	Status __cdecl recordCountCallback(void*,RecordType type,std::uint32_t* count){if(!count)return Status::InvalidArgument;switch(type){case RecordType::Attribute:*count=8;break;case RecordType::Skill:*count=27;break;case RecordType::Npc:case RecordType::Class:case RecordType::Race:case RecordType::Birthsign:*count=1;break;case RecordType::Spell:*count=2;break;case RecordType::MagicEffect:*count=3;break;default:return Status::Unsupported;}return Status::Ok;}
	Status __cdecl recordCallback(void*,RecordType type,std::uint32_t index,StringView id,RecordSnapshot* output){if(!output||output->structureSize!=sizeof(*output)||output->abiVersion!=BridgeAbiVersion)return Status::InvalidArgument;std::string key(id.data,id.size);std::transform(key.begin(),key.end(),key.begin(),[](unsigned char c){return static_cast<char>(std::tolower(c));});RecordSnapshot r{};r.structureSize=sizeof(r);r.abiVersion=BridgeAbiVersion;r.type=type;r.index=index;
		if(type==RecordType::Attribute){if(!key.empty()){index=8;for(unsigned i=0;i<8;++i)if(key==AttributeIds[i])index=i;}if(index>=8)return Status::NotFound;textValue(r.id,AttributeIds[index]);textValue(r.name,AttributeIds[index]);}
		else if(type==RecordType::Skill){if(!key.empty()){index=27;for(unsigned i=0;i<27;++i)if(key==SkillIds[i])index=i;}if(index>=27)return Status::NotFound;textValue(r.id,SkillIds[index]);textValue(r.name,SkillIds[index]);r.specialization=index<9?0:index<18?1:2;}
		else if(type==RecordType::Npc){if((!key.empty()&&key!="player")||(key.empty()&&index>0))return Status::NotFound;textValue(r.id,"player");textValue(r.name,"Test Player");textValue(r.auxiliaryId1,"dark elf");textValue(r.auxiliaryId2,"acrobat");textValue(r.itemIds[0],"the lady");r.flags=2;}
		else if(type==RecordType::Class){if((!key.empty()&&key!="acrobat")||(key.empty()&&index>0))return Status::NotFound;textValue(r.id,"acrobat");textValue(r.name,"Acrobat");r.specialization=2;r.itemCount[0]=2;r.itemCount[1]=5;r.itemCount[2]=5;textValue(r.itemIds[0],"agility");textValue(r.itemIds[1],"endurance");for(int i=0;i<10;++i)textValue(r.itemIds[2+i],SkillIds[i]);}
		else if(type==RecordType::Race){if((!key.empty()&&key!="dark elf")||(key.empty()&&index>0))return Status::NotFound;textValue(r.id,"dark elf");textValue(r.name,"Dark Elf");r.itemCount[0]=8;r.itemCount[1]=7;r.itemCount[2]=1;for(int i=0;i<8;++i){textValue(r.itemIds[i],AttributeIds[i]);r.itemValues[i*2]=40+i;r.itemValues[i*2+1]=38+i;}for(int i=0;i<7;++i){textValue(r.itemIds[8+i],SkillIds[i]);r.itemValues[16+i]=5;}textValue(r.itemIds[15],"ancestor guardian");}
		else if(type==RecordType::Birthsign){if((!key.empty()&&key!="the lady")||(key.empty()&&index>0))return Status::NotFound;textValue(r.id,"the lady");textValue(r.name,"The Lady");r.itemCount[0]=1;textValue(r.itemIds[0],"lady's favor");}
		else if(type==RecordType::Spell){const char* ids[]={"ancestor guardian","lady's favor"};if(!key.empty()){index=2;for(unsigned i=0;i<2;++i)if(key==ids[i])index=i;}if(index>=2)return Status::NotFound;textValue(r.id,ids[index]);textValue(r.name,ids[index]);r.specialization=0;r.value=5;r.flags=4;r.itemCount[0]=1;textValue(r.effects[0].id,"fortifyattribute");textValue(r.effects[0].affectedAttribute,"agility");r.effects[0].magnitudeMin=5;r.effects[0].magnitudeMax=5;}
		else if(type==RecordType::MagicEffect){const char* ids[]={"fortifyattribute","fortifyhealth","fortifyskill"};if(!key.empty()){index=3;for(unsigned i=0;i<3;++i)if(key==ids[i])index=i;}if(index>=3)return Status::NotFound;textValue(r.id,ids[index]);textValue(r.name,ids[index]);r.specialization=0;r.itemValues[0]=1;r.flags=0x1000;}
		else return Status::Unsupported;*output=r;return Status::Ok;}
	Status __cdecl actorSpellCountCallback(void* userData,OpaqueHandle handle,std::uint32_t* count){auto status=validateGameplayHandle(userData,handle,HandleType::Player);if(status!=Status::Ok)return status;if(!count)return Status::InvalidArgument;*count=1+(static_cast<GameplayFixture*>(userData)->spellAdded?1:0);return Status::Ok;}
	Status __cdecl actorSpellCallback(void* userData,OpaqueHandle handle,std::uint32_t index,RecordSnapshot* output){auto status=validateGameplayHandle(userData,handle,HandleType::Player);if(status!=Status::Ok)return status;if(index>=1+(static_cast<GameplayFixture*>(userData)->spellAdded?1u:0u))return Status::OutOfRange;const char* id=index?"lady's favor":"ancestor guardian";return recordCallback(userData,RecordType::Spell,0,{id,static_cast<std::uint32_t>(std::strlen(id))},output);}
	Status __cdecl setActorSpellCallback(void* userData,OpaqueHandle handle,StringView id,std::uint32_t add){auto status=validateGameplayHandle(userData,handle,HandleType::Player);if(status!=Status::Ok)return status;std::string key(id.data,id.size);if(key!="lady's favor")return Status::NotFound;static_cast<GameplayFixture*>(userData)->spellAdded=add!=0;return Status::Ok;}
	Status __cdecl activeSpellCountCallback(void* userData,OpaqueHandle handle,std::uint32_t* count){auto status=validateGameplayHandle(userData,handle,HandleType::Player);if(status!=Status::Ok)return status;if(!count)return Status::InvalidArgument;*count=1;return Status::Ok;}
	Status __cdecl activeSpellCallback(void* userData,OpaqueHandle handle,std::uint32_t index,ActiveSpellSnapshot* output){auto status=validateGameplayHandle(userData,handle,HandleType::Player);if(status!=Status::Ok)return status;if(index||!output||output->structureSize!=sizeof(*output)||output->abiVersion!=BridgeAbiVersion)return Status::OutOfRange;ActiveSpellSnapshot r{};r.structureSize=sizeof(r);r.abiVersion=BridgeAbiVersion;textValue(r.id,"lady's favor");r.affectsBaseValues=1;r.effectCount=1;textValue(r.effects[0].id,"fortifyhealth");r.effects[0].magnitudeThisFrame=10;*output=r;return Status::Ok;}
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
			&gameSettingCallback, &contentFileCountCallback, &contentFileCallback, &gameplay,
			&playerCallback,&validateGameplayHandle,&cellCallback,&statCallback,&setStatCallback,&recordCountCallback,&recordCallback,
			&actorSpellCountCallback,&actorSpellCallback,&setActorSpellCallback,&activeSpellCountCallback,&activeSpellCallback };
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

		writeFile(temporary / "gameplay.lua", R"(
local core=require('openmw.core')
local T=require('openmw.types')
local self=require('openmw.self')
assert(rawequal(self,require('openmw.self')) and self.recordId=='player')
assert(self.type==T.Player and T.Actor.objectIsInstance(self) and T.NPC.objectIsInstance(self) and T.Player.objectIsInstance(self))
assert(self.cell.name=='Balmora' and self.cell.isExterior and self.cell.gridX==1 and self.cell.worldSpaceId=='sys::default')
assert(#core.stats.Attribute.records==8 and core.stats.Attribute.records.strength.id=='strength')
assert(#core.stats.Skill.records==27 and core.stats.Skill.records.block.id=='block')
local npc=T.Player.record(self);assert(npc.name=='Test Player' and npc.race=='dark elf' and npc.class=='acrobat')
local class=T.NPC.classes.record('AcRoBaT');assert(class.specialization=='stealth' and #class.majorSkills==5)
local race=T.NPC.races.record('DARK ELF');assert(race.attributes.strength.male==40 and race.skills.block==5)
assert(T.Player.getBirthSign(self)=='the lady' and T.Player.birthSigns.record('THE LADY').spells[1]=="lady's favor")
assert(T.Player.isCharGenFinished(self))
local attr=T.Actor.stats.attributes.strength(self);local ab,am,ad=attr.base,attr.modifier,attr.damage
attr.base=ab+1;attr.modifier=2;attr.damage=1;assert(attr.base==ab+1 and attr.modifier==2 and attr.damage==1 and attr.modified==ab+2)
attr.base,attr.modifier,attr.damage=ab,am,ad
local skill=T.NPC.stats.skills.block(self);local sb,sm,sd,sp=skill.base,skill.modifier,skill.damage,skill.progress
skill.base=12;skill.modifier=3;skill.damage=1;skill.progress=.5;assert(skill.modified==14 and skill.progress==.5)
skill.base,skill.modifier,skill.damage,skill.progress=sb,sm,sd,sp
local level=T.Actor.stats.level(self);local lc,lp=level.current,level.progress;level.current=lc+1;level.progress=lp+2;assert(level.current==lc+1);level.current,level.progress=lc,lp
local health=T.Actor.stats.dynamic.health(self);local hb,hc,hm=health.base,health.current,health.modifier;health.base=hb+1;health.current=hc-1;health.modifier=2;assert(health.base==hb+1 and health.current==hc-1);health.base,health.current,health.modifier=hb,hc,hm
assert(not pcall(function() attr.modified=0 end) and not pcall(function() self.recordId='bad' end))
local playerSpells=T.Player.spells(self);assert(playerSpells[1].id=='ancestor guardian');playerSpells:add("lady's favor");assert(#T.Player.spells(self)==2);T.Player.spells(self):remove("lady's favor")
local active=T.Actor.activeSpells(self);assert(active[1].affectsBaseValues and active[1].effects[1].id=='fortifyhealth' and active[1].effects[1].magnitudeThisFrame==10)
assert(core.magic.spells.record('ANCESTOR GUARDIAN').id=='ancestor guardian' and core.magic.EFFECT_TYPE.FortifyAttribute=='fortifyattribute')
return {}
)");
		writeFile(temporary / "gameplay.omwscripts", "PLAYER: gameplay.lua\n");
		scripts="gameplay.omwscripts";content=scripts;auto gameplayConfig=configFor(temporary,scripts,content,InitializationEnabled|InitializationHarnessMode,temporary/"gameplay-reports");
		require(api.initialize(&gameplayConfig)==Status::Ok,"gameplay-runtime-initialize");
		frame={sizeof(FrameUpdate),BridgeAbiVersion,1,0.016,1.0,1.0,1.0,30.0,0,0};require(api.update(&frame)==Status::Ok,"gameplay-first-update");
		require(validateGameplayHandle(&gameplay,{1,gameplay.generation,static_cast<std::uint32_t>(HandleType::Cell)},HandleType::Player)==Status::WrongHandleType,"wrong-handle-type-rejected");
		const auto oldGeneration=gameplay.generation++;require(validateGameplayHandle(&gameplay,{1,oldGeneration,static_cast<std::uint32_t>(HandleType::Player)},HandleType::Player)==Status::StaleHandle,"stale-handle-rejected");
		require(api.reload()==Status::Ok,"gameplay-reload-new-generation");frame.frameNumber=2;require(api.update(&frame)==Status::Ok,"gameplay-update-after-reload");
		require(api.shutdown()==Status::Ok,"gameplay-clean-shutdown");

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
