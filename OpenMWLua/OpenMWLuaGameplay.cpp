#include "OpenMWLuaHost.h"

namespace mwse::openmw::host {
	namespace {
		constexpr const char* ObjectMetatable = "mwse.openmw.gameObject";
		constexpr const char* CellMetatable = "mwse.openmw.cell";
		constexpr const char* StatMetatable = "mwse.openmw.stat";
		constexpr const char* AttributeIds[] = { "strength", "intelligence", "willpower", "agility", "speed", "endurance", "personality", "luck" };
		constexpr const char* SkillIds[] = { "block", "armorer", "mediumarmor", "heavyarmor", "bluntweapon", "longblade", "axe", "spear", "athletics", "enchant", "destruction", "alteration", "illusion", "conjuration", "mysticism", "restoration", "alchemy", "unarmored", "security", "sneak", "acrobatics", "lightarmor", "shortblade", "marksman", "mercantile", "speechcraft", "handtohand" };
		constexpr const char* Specializations[] = { "combat", "magic", "stealth" };
		constexpr const char* Schools[] = { "alteration", "conjuration", "destruction", "illusion", "mysticism", "restoration" };

		struct ObjectUserdata { Host* host; OpaqueHandle handle; };
		struct CellUserdata { Host* host; OpaqueHandle handle; };
		struct StatUserdata { Host* host; OpaqueHandle handle; StatKind kind; std::uint32_t index; };

		std::string text(const BridgeText& value) {
			if (value.size > BridgeTextCapacity) throw std::runtime_error("native bridge returned an invalid bounded string");
			return std::string(value.data, value.size);
		}

		void pushText(lua_State* state, const BridgeText& value) {
			if (value.size > BridgeTextCapacity) luaL_error(state, "native bridge returned an invalid bounded string");
			lua_pushlstring(state, value.data, value.size);
		}

		ObjectUserdata* checkObject(lua_State* state, int index) {
			return static_cast<ObjectUserdata*>(luaL_checkudata(state, index, ObjectMetatable));
		}

		int statusError(lua_State* state, Status status, std::string_view operation) {
			return luaL_error(state, "%.*s failed with bridge status %u", static_cast<int>(operation.size()), operation.data(), static_cast<unsigned>(status));
		}

		void setFunction(lua_State* state, int table, const char* name, lua_CFunction function, Host* host,
			std::optional<lua_Integer> selector = {}) {
			table = table > 0 ? table : lua_gettop(state) + table + 1;
			lua_pushlightuserdata(state, host);
			int upvalues = 1;
			if (selector) { lua_pushinteger(state, *selector); ++upvalues; }
			lua_pushcclosure(state, function, upvalues);
			lua_setfield(state, table, name);
		}

		void pushStringArray(lua_State* state, const BridgeText* values, std::uint32_t count) {
			lua_createtable(state, count, 0);
			for (std::uint32_t i = 0; i < count; ++i) { pushText(state, values[i]); lua_rawseti(state, -2, i + 1); }
		}

		void ensureMetatables(lua_State* state) {
			if (luaL_newmetatable(state, ObjectMetatable)) {
				lua_pushcfunction(state, &Host::objectIndexThunk); lua_setfield(state, -2, "__index");
				lua_pushcfunction(state, &Host::objectNewIndexThunk); lua_setfield(state, -2, "__newindex");
				lua_pushliteral(state, "GameObject"); lua_setfield(state, -2, "__name");
				lua_pushboolean(state, 0); lua_setfield(state, -2, "__metatable");
			}
			lua_pop(state, 1);
			if (luaL_newmetatable(state, CellMetatable)) {
				lua_pushcfunction(state, &Host::cellIndexThunk); lua_setfield(state, -2, "__index");
				lua_pushcfunction(state, &Host::objectNewIndexThunk); lua_setfield(state, -2, "__newindex");
				lua_pushboolean(state, 0); lua_setfield(state, -2, "__metatable");
			}
			lua_pop(state, 1);
			if (luaL_newmetatable(state, StatMetatable)) {
				lua_pushcfunction(state, &Host::statIndexThunk); lua_setfield(state, -2, "__index");
				lua_pushcfunction(state, &Host::statNewIndexThunk); lua_setfield(state, -2, "__newindex");
				lua_pushboolean(state, 0); lua_setfield(state, -2, "__metatable");
			}
			lua_pop(state, 1);
		}
	}

	int Host::proxyLenThunk(lua_State* state) {
		lua_pushinteger(state, static_cast<lua_Integer>(lua_objlen(state, lua_upvalueindex(1))));
		return 1;
	}

	int Host::compatibilityInvalidHandleProbeThunk(lua_State* state) {
		auto* host=static_cast<Host*>(lua_touserdata(state,lua_upvalueindex(1)));if(!host||(host->mConfig.flags&InitializationHarnessMode)==0)return luaL_error(state,"handle probes are available only in harness mode");ObjectSnapshot object{};object.structureSize=sizeof(object);object.abiVersion=BridgeAbiVersion;auto status=host->mCallbacks.getPlayerObject(host->mCallbacks.gameUserData,&object);if(status!=Status::Ok)return statusError(state,status,"handle probe acquisition");OpaqueHandle wrong=object.object;wrong.reserved=static_cast<std::uint32_t>(HandleType::Cell);const auto wrongStatus=host->mCallbacks.validateHandle(host->mCallbacks.gameUserData,wrong,HandleType::Player);OpaqueHandle stale=object.object;if(stale.generation>0)--stale.generation;else ++stale.generation;const auto staleStatus=host->mCallbacks.validateHandle(host->mCallbacks.gameUserData,stale,HandleType::Player);host->mRejectedHandles+=2;lua_newtable(state);lua_pushinteger(state,static_cast<lua_Integer>(wrongStatus));lua_setfield(state,-2,"wrongType");lua_pushinteger(state,static_cast<lua_Integer>(staleStatus));lua_setfield(state,-2,"stale");return 1;
	}

	void Host::pushPlayerObject() {
		ensureMetatables(mLua);
		if (mPlayerObjectReference != LUA_NOREF) { lua_rawgeti(mLua, LUA_REGISTRYINDEX, mPlayerObjectReference); return; }
		ObjectSnapshot snapshot{}; snapshot.structureSize = sizeof(snapshot); snapshot.abiVersion = BridgeAbiVersion;
		const Status status = mCallbacks.getPlayerObject(mCallbacks.gameUserData, &snapshot);
		++mGameplayCalls;
		if (status != Status::Ok) throw std::runtime_error("native player handle acquisition failed with bridge status " + std::to_string(static_cast<unsigned>(status)));
		if (snapshot.structureSize != sizeof(snapshot) || snapshot.abiVersion != BridgeAbiVersion
			|| snapshot.object.reserved != static_cast<std::uint32_t>(HandleType::Player)) throw std::runtime_error("native player snapshot failed layout validation");
		auto* object = static_cast<ObjectUserdata*>(lua_newuserdata(mLua, sizeof(ObjectUserdata)));
		*object = { this, snapshot.object };
		luaL_getmetatable(mLua, ObjectMetatable); lua_setmetatable(mLua, -2);
		lua_pushvalue(mLua, -1); mPlayerObjectReference = luaL_ref(mLua, LUA_REGISTRYINDEX);
	}

	int Host::objectIndexThunk(lua_State* state) {
		auto* object = checkObject(state, 1);
		if (!lua_isstring(state, 2) || !object || !object->host) return luaL_error(state, "invalid GameObject access");
		auto* host = object->host; ++host->mGameplayCalls;
		const Status validation = host->mCallbacks.validateHandle(host->mCallbacks.gameUserData, object->handle, HandleType::Player);
		if (validation != Status::Ok) { ++host->mRejectedHandles; return statusError(state, validation, "player handle validation"); }
		const char* key = lua_tostring(state, 2);
		if (std::strcmp(key, "_mwseFoundationAvailable") == 0 || std::strcmp(key, "isValid") == 0) { lua_pushboolean(state, 1); return 1; }
		if (std::strcmp(key, "sendEvent") == 0) { lua_pushlightuserdata(state, host); lua_pushcclosure(state, &objectSendEventThunk, 1); return 1; }
		ObjectSnapshot snapshot{}; snapshot.structureSize=sizeof(snapshot);snapshot.abiVersion=BridgeAbiVersion;
		const Status status=host->mCallbacks.getPlayerObject(host->mCallbacks.gameUserData,&snapshot);if(status!=Status::Ok)return statusError(state,status,"player snapshot");
		if (std::strcmp(key, "recordId") == 0) { pushText(state, snapshot.recordId); return 1; }
		if (std::strcmp(key, "type") == 0) { if(host->mPlayerTypeReference==LUA_NOREF) return luaL_error(state,"openmw.types is not initialized"); lua_rawgeti(state,LUA_REGISTRYINDEX,host->mPlayerTypeReference);return 1; }
		if (std::strcmp(key, "cell") == 0) {
			if(snapshot.cell.value==0){lua_pushnil(state);return 1;}
			const std::uint64_t cacheKey=(static_cast<std::uint64_t>(snapshot.cell.generation)<<32)^snapshot.cell.value;
			if(auto it=host->mCellReferences.find(cacheKey);it!=host->mCellReferences.end()){lua_rawgeti(state,LUA_REGISTRYINDEX,it->second);return 1;}
			auto* cell=static_cast<CellUserdata*>(lua_newuserdata(state,sizeof(CellUserdata)));*cell={host,snapshot.cell};luaL_getmetatable(state,CellMetatable);lua_setmetatable(state,-2);lua_pushvalue(state,-1);host->mCellReferences[cacheKey]=luaL_ref(state,LUA_REGISTRYINDEX);return 1;
		}
		lua_pushnil(state);return 1;
	}

	int Host::objectNewIndexThunk(lua_State* state) { return luaL_error(state, "attempt to modify a read-only OpenMW object property"); }

	int Host::cellIndexThunk(lua_State* state) {
		auto* cell=static_cast<CellUserdata*>(luaL_checkudata(state,1,CellMetatable));if(!cell||!cell->host||!lua_isstring(state,2))return luaL_error(state,"invalid Cell access");auto* host=cell->host;++host->mGameplayCalls;
		CellSnapshot snapshot{};snapshot.structureSize=sizeof(snapshot);snapshot.abiVersion=BridgeAbiVersion;const auto status=host->mCallbacks.getCell(host->mCallbacks.gameUserData,cell->handle,&snapshot);if(status!=Status::Ok){if(status==Status::StaleHandle||status==Status::WrongHandleType||status==Status::InvalidHandle)++host->mRejectedHandles;return statusError(state,status,"cell snapshot");}
		const char* key=lua_tostring(state,2);
		if(std::strcmp(key,"id")==0)pushText(state,snapshot.id);else if(std::strcmp(key,"name")==0)pushText(state,snapshot.name);else if(std::strcmp(key,"displayName")==0)pushText(state,snapshot.displayName);else if(std::strcmp(key,"region")==0){if(snapshot.region.size)pushText(state,snapshot.region);else lua_pushnil(state);}else if(std::strcmp(key,"worldSpaceId")==0)pushText(state,snapshot.worldSpaceId);else if(std::strcmp(key,"gridX")==0)lua_pushinteger(state,snapshot.gridX);else if(std::strcmp(key,"gridY")==0)lua_pushinteger(state,snapshot.gridY);else if(std::strcmp(key,"waterLevel")==0){if(snapshot.flags&CellHasWaterLevel)lua_pushnumber(state,snapshot.waterLevel);else lua_pushnil(state);}else if(std::strcmp(key,"hasWater")==0)lua_pushboolean(state,(snapshot.flags&CellHasWater)!=0);else if(std::strcmp(key,"hasSky")==0)lua_pushboolean(state,(snapshot.flags&CellHasSky)!=0);else if(std::strcmp(key,"isExterior")==0)lua_pushboolean(state,(snapshot.flags&CellIsExterior)!=0);else if(std::strcmp(key,"isQuasiExterior")==0)lua_pushboolean(state,(snapshot.flags&CellIsQuasiExterior)!=0);else lua_pushnil(state);return 1;
	}

	int Host::typeObjectIsInstanceThunk(lua_State* state) {
		auto* host=static_cast<Host*>(lua_touserdata(state,lua_upvalueindex(1)));const auto mask=static_cast<std::uint32_t>(lua_tointeger(state,lua_upvalueindex(2)));auto* object=checkObject(state,1);if(!host||!object||object->host!=host)return luaL_error(state,"objectIsInstance expects a GameObject from this runtime");
		const auto validation=host->mCallbacks.validateHandle(host->mCallbacks.gameUserData,object->handle,HandleType::Player);++host->mGameplayCalls;if(validation!=Status::Ok){++host->mRejectedHandles;return statusError(state,validation,"object type handle validation");}
		ObjectSnapshot snapshot{};snapshot.structureSize=sizeof(snapshot);snapshot.abiVersion=BridgeAbiVersion;const auto status=host->mCallbacks.getPlayerObject(host->mCallbacks.gameUserData,&snapshot);++host->mGameplayCalls;if(status!=Status::Ok)return statusError(state,status,"object type detection");lua_pushboolean(state,(snapshot.typeMask&mask)==mask);return 1;
	}

	int Host::statGetterThunk(lua_State* state) {
		auto* host=static_cast<Host*>(lua_touserdata(state,lua_upvalueindex(1)));const auto kind=static_cast<StatKind>(lua_tointeger(state,lua_upvalueindex(2)));const auto index=static_cast<std::uint32_t>(lua_tointeger(state,lua_upvalueindex(3)));auto* object=checkObject(state,1);if(!host||!object||object->host!=host)return luaL_error(state,"stat getter expects the player GameObject");
		const auto validation=host->mCallbacks.validateHandle(host->mCallbacks.gameUserData,object->handle,HandleType::Player);++host->mGameplayCalls;if(validation!=Status::Ok){++host->mRejectedHandles;return statusError(state,validation,"stat getter handle validation");}
		auto* stat=static_cast<StatUserdata*>(lua_newuserdata(state,sizeof(StatUserdata)));*stat={host,object->handle,kind,index};luaL_getmetatable(state,StatMetatable);lua_setmetatable(state,-2);return 1;
	}

	int Host::statIndexThunk(lua_State* state) {
		auto* stat=static_cast<StatUserdata*>(luaL_checkudata(state,1,StatMetatable));if(!stat||!stat->host||!lua_isstring(state,2))return luaL_error(state,"invalid stat proxy access");
		StatSnapshot snapshot{};snapshot.structureSize=sizeof(snapshot);snapshot.abiVersion=BridgeAbiVersion;const auto status=stat->host->mCallbacks.getStat(stat->host->mCallbacks.gameUserData,stat->handle,stat->kind,stat->index,&snapshot);++stat->host->mGameplayCalls;if(status!=Status::Ok)return statusError(state,status,"stat read");
		const char* key=lua_tostring(state,2);if(std::strcmp(key,"base")==0)lua_pushnumber(state,snapshot.base);else if(std::strcmp(key,"current")==0)lua_pushnumber(state,snapshot.current);else if(std::strcmp(key,"modified")==0)lua_pushnumber(state,snapshot.modified);else if(std::strcmp(key,"modifier")==0)lua_pushnumber(state,snapshot.modifier);else if(std::strcmp(key,"damage")==0)lua_pushnumber(state,snapshot.damage);else if(std::strcmp(key,"progress")==0)lua_pushnumber(state,snapshot.progress);else lua_pushnil(state);return 1;
	}

	int Host::statNewIndexThunk(lua_State* state) {
		auto* stat=static_cast<StatUserdata*>(luaL_checkudata(state,1,StatMetatable));if(!stat||!stat->host||!lua_isstring(state,2)||!lua_isnumber(state,3))return luaL_error(state,"stat writes require a named numeric field");const char* key=lua_tostring(state,2);StatField field;
		if(std::strcmp(key,"base")==0)field=StatField::Base;else if(std::strcmp(key,"current")==0)field=StatField::Current;else if(std::strcmp(key,"modifier")==0)field=StatField::Modifier;else if(std::strcmp(key,"damage")==0)field=StatField::Damage;else if(std::strcmp(key,"progress")==0)field=StatField::Progress;else return luaL_error(state,"attempt to modify a read-only OpenMW stat field");
		const auto status=stat->host->mCallbacks.setStat(stat->host->mCallbacks.gameUserData,stat->handle,stat->kind,stat->index,field,lua_tonumber(state,3));++stat->host->mGameplayCalls;if(status!=Status::Ok)return statusError(state,status,"stat write");return 0;
	}

	void Host::pushEffect(const EffectSnapshot& snapshot, bool active) {
		lua_newtable(mLua);pushText(mLua,snapshot.id);lua_setfield(mLua,-2,"id");
		if(snapshot.affectedSkill.size){pushText(mLua,snapshot.affectedSkill);lua_setfield(mLua,-2,"affectedSkill");}
		if(snapshot.affectedAttribute.size){pushText(mLua,snapshot.affectedAttribute);lua_setfield(mLua,-2,"affectedAttribute");}
		lua_pushnumber(mLua,snapshot.magnitudeMin);lua_setfield(mLua,-2,"magnitudeMin");lua_pushnumber(mLua,snapshot.magnitudeMax);lua_setfield(mLua,-2,"magnitudeMax");lua_pushnumber(mLua,snapshot.duration);lua_setfield(mLua,-2,"duration");lua_pushnumber(mLua,snapshot.area);lua_setfield(mLua,-2,"area");lua_pushinteger(mLua,snapshot.range);lua_setfield(mLua,-2,"range");
		if(active){lua_pushnumber(mLua,snapshot.magnitudeThisFrame);lua_setfield(mLua,-2,"magnitudeThisFrame");}
		RecordSnapshot effect{};effect.structureSize=sizeof(effect);effect.abiVersion=BridgeAbiVersion;const std::string id=text(snapshot.id);const StringView view{id.data(),static_cast<std::uint32_t>(id.size())};if(mCallbacks.getRecord(mCallbacks.gameUserData,RecordType::MagicEffect,0,view,&effect)==Status::Ok){pushRecord(effect);lua_setfield(mLua,-2,"effect");}
		pushReadOnlyProxy(-1,false);lua_remove(mLua,-2);
	}

	void Host::pushRecord(const RecordSnapshot& record) {
		lua_newtable(mLua);const int value=lua_gettop(mLua);pushText(mLua,record.id);lua_setfield(mLua,value,"id");if(record.name.size){pushText(mLua,record.name);lua_setfield(mLua,value,"name");}
		if(record.type==RecordType::Attribute){}
		else if(record.type==RecordType::Skill){if(record.specialization>=0&&record.specialization<3){lua_pushstring(mLua,Specializations[record.specialization]);lua_setfield(mLua,value,"specialization");}}
		else if(record.type==RecordType::Npc){pushText(mLua,record.auxiliaryId1);lua_setfield(mLua,value,"race");pushText(mLua,record.auxiliaryId2);lua_setfield(mLua,value,"class");lua_pushboolean(mLua,(record.flags&1u)==0);lua_setfield(mLua,value,"isMale");}
		else if(record.type==RecordType::Class){pushStringArray(mLua,record.itemIds,record.itemCount[0]);lua_setfield(mLua,value,"attributes");pushStringArray(mLua,record.itemIds+2,record.itemCount[1]);lua_setfield(mLua,value,"majorSkills");pushStringArray(mLua,record.itemIds+7,record.itemCount[2]);lua_setfield(mLua,value,"minorSkills");if(record.specialization>=0&&record.specialization<3){lua_pushstring(mLua,Specializations[record.specialization]);lua_setfield(mLua,value,"specialization");}}
		else if(record.type==RecordType::Race){lua_newtable(mLua);for(std::uint32_t i=0;i<record.itemCount[0];++i){lua_newtable(mLua);lua_pushnumber(mLua,record.itemValues[i*2]);lua_setfield(mLua,-2,"male");lua_pushnumber(mLua,record.itemValues[i*2+1]);lua_setfield(mLua,-2,"female");pushText(mLua,record.itemIds[i]);lua_insert(mLua,-2);lua_settable(mLua,-3);}lua_setfield(mLua,value,"attributes");lua_newtable(mLua);for(std::uint32_t i=0;i<record.itemCount[1];++i){pushText(mLua,record.itemIds[8+i]);lua_pushnumber(mLua,record.itemValues[16+i]);lua_settable(mLua,-3);}lua_setfield(mLua,value,"skills");pushStringArray(mLua,record.itemIds+15,record.itemCount[2]);lua_setfield(mLua,value,"spells");}
		else if(record.type==RecordType::Birthsign){pushStringArray(mLua,record.itemIds,record.itemCount[0]);lua_setfield(mLua,value,"spells");}
		else if(record.type==RecordType::Spell){lua_pushinteger(mLua,record.specialization);lua_setfield(mLua,value,"type");lua_pushinteger(mLua,record.value);lua_setfield(mLua,value,"cost");lua_pushboolean(mLua,(record.flags&1u)!=0);lua_setfield(mLua,value,"autocalcFlag");lua_pushboolean(mLua,(record.flags&2u)!=0);lua_setfield(mLua,value,"alwaysSucceedFlag");lua_pushboolean(mLua,(record.flags&4u)!=0);lua_setfield(mLua,value,"starterSpellFlag");lua_createtable(mLua,record.itemCount[0],0);for(std::uint32_t i=0;i<record.itemCount[0];++i){pushEffect(record.effects[i],false);lua_rawseti(mLua,-2,i+1);}lua_setfield(mLua,value,"effects");}
		else if(record.type==RecordType::MagicEffect){lua_pushnumber(mLua,record.itemValues[0]);lua_setfield(mLua,value,"baseCost");if(record.specialization>=0&&record.specialization<6){lua_pushstring(mLua,Schools[record.specialization]);lua_setfield(mLua,value,"school");}lua_pushboolean(mLua,(record.flags&0x8u)==0);lua_setfield(mLua,value,"hasMagnitude");lua_pushboolean(mLua,(record.flags&0x4u)==0);lua_setfield(mLua,value,"hasDuration");lua_pushboolean(mLua,(record.flags&0x1000u)!=0);lua_setfield(mLua,value,"isAppliedOnce");}
		pushReadOnlyProxy(value,false);lua_remove(mLua,value);
	}

	int Host::recordLookupThunk(lua_State* state) {
		auto* host=static_cast<Host*>(lua_touserdata(state,lua_upvalueindex(1)));const auto type=static_cast<RecordType>(lua_tointeger(state,lua_upvalueindex(2)));if(!host)return luaL_error(state,"invalid record collection handle");std::string id;std::uint32_t index=0;
		if(type==RecordType::Npc){auto* object=checkObject(state,1);if(!object||object->host!=host)return luaL_error(state,"NPC.record expects a GameObject");const auto validation=host->mCallbacks.validateHandle(host->mCallbacks.gameUserData,object->handle,HandleType::Player);++host->mGameplayCalls;if(validation!=Status::Ok){++host->mRejectedHandles;return statusError(state,validation,"NPC record handle validation");}}
		else if(lua_isnumber(state,1))index=static_cast<std::uint32_t>(lua_tointeger(state,1)-1);else{size_t size=0;const char* value=luaL_checklstring(state,1,&size);id.assign(value,size);}
		RecordSnapshot snapshot{};snapshot.structureSize=sizeof(snapshot);snapshot.abiVersion=BridgeAbiVersion;const StringView view{id.data(),static_cast<std::uint32_t>(id.size())};const auto status=host->mCallbacks.getRecord(host->mCallbacks.gameUserData,type,index,view,&snapshot);++host->mGameplayCalls;if(status==Status::NotFound){lua_pushnil(state);return 1;}if(status!=Status::Ok)return statusError(state,status,"record lookup");host->pushRecord(snapshot);return 1;
	}

	void Host::pushRecordCollection(RecordType type) {
		lua_newtable(mLua);const int collection=lua_gettop(mLua);std::uint32_t count=0;const auto countStatus=mCallbacks.getRecordCount(mCallbacks.gameUserData,type,&count);if(countStatus!=Status::Ok)throw std::runtime_error("record count callback failed");lua_createtable(mLua,count,count);const int records=lua_gettop(mLua);
		for(std::uint32_t i=0;i<count;++i){RecordSnapshot snapshot{};snapshot.structureSize=sizeof(snapshot);snapshot.abiVersion=BridgeAbiVersion;const auto status=mCallbacks.getRecord(mCallbacks.gameUserData,type,i,{},&snapshot);if(status!=Status::Ok)throw std::runtime_error("record enumeration failed for type "+std::to_string(static_cast<unsigned>(type))+" index "+std::to_string(i)+" with bridge status "+std::to_string(static_cast<unsigned>(status)));pushRecord(snapshot);lua_pushvalue(mLua,-1);lua_rawseti(mLua,records,i+1);pushText(mLua,snapshot.id);lua_insert(mLua,-2);lua_settable(mLua,records);}
		pushReadOnlyProxy(records,false);lua_remove(mLua,records);lua_setfield(mLua,collection,"records");setFunction(mLua,collection,"record",&recordLookupThunk,this,static_cast<lua_Integer>(type));pushReadOnlyProxy(collection,false);lua_remove(mLua,collection);
	}

	int Host::activeSpellsThunk(lua_State* state) {
		auto* host=static_cast<Host*>(lua_touserdata(state,lua_upvalueindex(1)));auto* object=checkObject(state,1);if(!host||!object||object->host!=host)return luaL_error(state,"activeSpells expects the player GameObject");std::uint32_t count=0;auto status=host->mCallbacks.getActiveSpellCount(host->mCallbacks.gameUserData,object->handle,&count);if(status!=Status::Ok)return statusError(state,status,"active spell count");lua_createtable(state,count,0);
		for(std::uint32_t i=0;i<count;++i){ActiveSpellSnapshot spell{};spell.structureSize=sizeof(spell);spell.abiVersion=BridgeAbiVersion;status=host->mCallbacks.getActiveSpell(host->mCallbacks.gameUserData,object->handle,i,&spell);if(status!=Status::Ok)return statusError(state,status,"active spell");lua_newtable(state);pushText(state,spell.id);lua_setfield(state,-2,"id");lua_pushboolean(state,spell.affectsBaseValues!=0);lua_setfield(state,-2,"affectsBaseValues");lua_createtable(state,spell.effectCount,0);for(std::uint32_t j=0;j<spell.effectCount;++j){host->pushEffect(spell.effects[j],true);lua_rawseti(state,-2,j+1);}lua_setfield(state,-2,"effects");host->pushReadOnlyProxy(-1,false);lua_remove(state,-2);lua_rawseti(state,-2,i+1);}return 1;
	}

	int Host::actorSpellMutationThunk(lua_State* state) {
		auto* host=static_cast<Host*>(lua_touserdata(state,lua_upvalueindex(1)));const bool add=lua_toboolean(state,lua_upvalueindex(2))!=0;auto* object=static_cast<ObjectUserdata*>(lua_touserdata(state,lua_upvalueindex(3)));const int stringIndex=lua_isstring(state,2)?2:1;size_t size=0;const char* id=luaL_checklstring(state,stringIndex,&size);if(!host||!object)return luaL_error(state,"invalid actor spell collection");const StringView view{id,static_cast<std::uint32_t>(size)};const auto status=host->mCallbacks.setActorSpell(host->mCallbacks.gameUserData,object->handle,view,add?1u:0u);if(status!=Status::Ok)return statusError(state,status,add?"add actor spell":"remove actor spell");lua_pushboolean(state,1);return 1;
	}

	int Host::actorSpellsThunk(lua_State* state) {
		auto* host=static_cast<Host*>(lua_touserdata(state,lua_upvalueindex(1)));auto* object=checkObject(state,1);if(!host||!object||object->host!=host)return luaL_error(state,"spells expects the player GameObject");std::uint32_t count=0;auto status=host->mCallbacks.getActorSpellCount(host->mCallbacks.gameUserData,object->handle,&count);if(status!=Status::Ok)return statusError(state,status,"actor spell count");lua_createtable(state,count,2);const int list=lua_gettop(state);
		for(std::uint32_t i=0;i<count;++i){RecordSnapshot spell{};spell.structureSize=sizeof(spell);spell.abiVersion=BridgeAbiVersion;status=host->mCallbacks.getActorSpell(host->mCallbacks.gameUserData,object->handle,i,&spell);if(status!=Status::Ok)return statusError(state,status,"actor spell");host->pushRecord(spell);lua_rawseti(state,list,i+1);}
		lua_pushlightuserdata(state,host);lua_pushboolean(state,1);lua_pushlightuserdata(state,object);lua_pushcclosure(state,&actorSpellMutationThunk,3);lua_setfield(state,list,"add");
		lua_pushlightuserdata(state,host);lua_pushboolean(state,0);lua_pushlightuserdata(state,object);lua_pushcclosure(state,&actorSpellMutationThunk,3);lua_setfield(state,list,"remove");return 1;
	}

	int Host::birthsignThunk(lua_State* state) {
		auto* host=static_cast<Host*>(lua_touserdata(state,lua_upvalueindex(1)));auto* object=checkObject(state,1);if(!host||!object||object->host!=host)return luaL_error(state,"getBirthSign expects the player GameObject");const auto validation=host->mCallbacks.validateHandle(host->mCallbacks.gameUserData,object->handle,HandleType::Player);++host->mGameplayCalls;if(validation!=Status::Ok){++host->mRejectedHandles;return statusError(state,validation,"birth sign handle validation");}RecordSnapshot record{};record.structureSize=sizeof(record);record.abiVersion=BridgeAbiVersion;const auto status=host->mCallbacks.getRecord(host->mCallbacks.gameUserData,RecordType::Npc,0,{},&record);if(status!=Status::Ok)return statusError(state,status,"player birth sign");if(record.itemIds[0].size)pushText(state,record.itemIds[0]);else lua_pushnil(state);return 1;
	}

	int Host::charGenFinishedThunk(lua_State* state) {
		auto* host=static_cast<Host*>(lua_touserdata(state,lua_upvalueindex(1)));auto* object=checkObject(state,1);if(!host||!object||object->host!=host)return luaL_error(state,"isCharGenFinished expects the player GameObject");const auto validation=host->mCallbacks.validateHandle(host->mCallbacks.gameUserData,object->handle,HandleType::Player);++host->mGameplayCalls;if(validation!=Status::Ok){++host->mRejectedHandles;return statusError(state,validation,"character generation handle validation");}RecordSnapshot record{};record.structureSize=sizeof(record);record.abiVersion=BridgeAbiVersion;const auto status=host->mCallbacks.getRecord(host->mCallbacks.gameUserData,RecordType::Npc,0,{},&record);if(status!=Status::Ok)return statusError(state,status,"character generation state");lua_pushboolean(state,(record.flags&2u)!=0);return 1;
	}

	void Host::pushStatFunctions(lua_State*, bool includeSkills) {
		lua_newtable(mLua);const int stats=lua_gettop(mLua);lua_newtable(mLua);const int attributes=lua_gettop(mLua);for(std::uint32_t i=0;i<std::size(AttributeIds);++i){lua_pushlightuserdata(mLua,this);lua_pushinteger(mLua,static_cast<lua_Integer>(StatKind::Attribute));lua_pushinteger(mLua,i);lua_pushcclosure(mLua,&statGetterThunk,3);lua_setfield(mLua,attributes,AttributeIds[i]);}lua_setfield(mLua,stats,"attributes");
		if(includeSkills){lua_newtable(mLua);const int skills=lua_gettop(mLua);for(std::uint32_t i=0;i<std::size(SkillIds);++i){lua_pushlightuserdata(mLua,this);lua_pushinteger(mLua,static_cast<lua_Integer>(StatKind::Skill));lua_pushinteger(mLua,i);lua_pushcclosure(mLua,&statGetterThunk,3);lua_setfield(mLua,skills,SkillIds[i]);}lua_setfield(mLua,stats,"skills");}
		lua_pushlightuserdata(mLua,this);lua_pushinteger(mLua,static_cast<lua_Integer>(StatKind::Level));lua_pushinteger(mLua,0);lua_pushcclosure(mLua,&statGetterThunk,3);lua_setfield(mLua,stats,"level");lua_newtable(mLua);lua_pushlightuserdata(mLua,this);lua_pushinteger(mLua,static_cast<lua_Integer>(StatKind::Health));lua_pushinteger(mLua,0);lua_pushcclosure(mLua,&statGetterThunk,3);lua_setfield(mLua,-2,"health");lua_setfield(mLua,stats,"dynamic");
	}

	void Host::pushTypesPackage(ScriptInstance&) {
		if(mTypesPackageReference!=LUA_NOREF){lua_rawgeti(mLua,LUA_REGISTRYINDEX,mTypesPackageReference);return;}
		ensureMetatables(mLua);lua_newtable(mLua);const int package=lua_gettop(mLua);
		auto makeType=[&](const char* name,std::uint32_t mask,bool npc,bool player){lua_newtable(mLua);const int type=lua_gettop(mLua);setFunction(mLua,type,"objectIsInstance",&typeObjectIsInstanceThunk,this,mask);pushStatFunctions(mLua,npc);lua_setfield(mLua,type,"stats");setFunction(mLua,type,"activeSpells",&activeSpellsThunk,this);if(npc){setFunction(mLua,type,"record",&recordLookupThunk,this,static_cast<lua_Integer>(RecordType::Npc));pushRecordCollection(RecordType::Class);lua_setfield(mLua,type,"classes");pushRecordCollection(RecordType::Race);lua_setfield(mLua,type,"races");}if(player){pushRecordCollection(RecordType::Birthsign);lua_setfield(mLua,type,"birthSigns");setFunction(mLua,type,"getBirthSign",&birthsignThunk,this);setFunction(mLua,type,"isCharGenFinished",&charGenFinishedThunk,this);setFunction(mLua,type,"spells",&actorSpellsThunk,this);}pushReadOnlyProxy(type,false);lua_remove(mLua,type);lua_pushvalue(mLua,-1);const int reference=luaL_ref(mLua,LUA_REGISTRYINDEX);lua_setfield(mLua,package,name);return reference;};
		mActorTypeReference=makeType("Actor",static_cast<std::uint32_t>(ObjectType::Actor),false,false);mNpcTypeReference=makeType("NPC",static_cast<std::uint32_t>(ObjectType::Npc),true,false);mPlayerTypeReference=makeType("Player",static_cast<std::uint32_t>(ObjectType::Player),true,true);pushReadOnlyProxy(package,false);lua_remove(mLua,package);lua_pushvalue(mLua,-1);mTypesPackageReference=luaL_ref(mLua,LUA_REGISTRYINDEX);
	}

	void Host::pushCoreGameplay() {
		lua_newtable(mLua);const int stats=lua_gettop(mLua);lua_newtable(mLua);pushRecordCollection(RecordType::Attribute);lua_getfield(mLua,-1,"records");lua_setfield(mLua,-3,"records");lua_pop(mLua,1);pushReadOnlyProxy(-1,false);lua_remove(mLua,-2);lua_setfield(mLua,stats,"Attribute");lua_newtable(mLua);pushRecordCollection(RecordType::Skill);lua_getfield(mLua,-1,"records");lua_setfield(mLua,-3,"records");lua_pop(mLua,1);pushReadOnlyProxy(-1,false);lua_remove(mLua,-2);lua_setfield(mLua,stats,"Skill");pushReadOnlyProxy(stats,false);lua_remove(mLua,stats);lua_setfield(mLua,-2,"stats");
		lua_newtable(mLua);const int magic=lua_gettop(mLua);lua_newtable(mLua);lua_pushinteger(mLua,0);lua_setfield(mLua,-2,"Self");lua_pushinteger(mLua,1);lua_setfield(mLua,-2,"Touch");lua_pushinteger(mLua,2);lua_setfield(mLua,-2,"Target");pushReadOnlyProxy(-1,true);lua_remove(mLua,-2);lua_setfield(mLua,magic,"RANGE");lua_newtable(mLua);for(int i=0;i<6;++i){lua_pushinteger(mLua,i);const char* names[]={"Spell","Ability","Blight","Disease","Curse","Power"};lua_setfield(mLua,-2,names[i]);}pushReadOnlyProxy(-1,true);lua_remove(mLua,-2);lua_setfield(mLua,magic,"SPELL_TYPE");lua_newtable(mLua);lua_pushliteral(mLua,"fortifyattribute");lua_setfield(mLua,-2,"FortifyAttribute");lua_pushliteral(mLua,"fortifyhealth");lua_setfield(mLua,-2,"FortifyHealth");lua_pushliteral(mLua,"fortifyskill");lua_setfield(mLua,-2,"FortifySkill");pushReadOnlyProxy(-1,true);lua_remove(mLua,-2);lua_setfield(mLua,magic,"EFFECT_TYPE");pushRecordCollection(RecordType::Spell);lua_setfield(mLua,magic,"spells");pushRecordCollection(RecordType::MagicEffect);lua_setfield(mLua,magic,"magicEffects");pushReadOnlyProxy(magic,false);lua_remove(mLua,magic);lua_setfield(mLua,-2,"magic");
	}
}
