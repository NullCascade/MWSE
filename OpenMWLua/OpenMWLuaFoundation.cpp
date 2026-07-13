#include "OpenMWLuaHost.h"

namespace mwse::openmw::host {

	namespace {
		constexpr char Vector2Metatable[] = "mwse.openmw.Vector2";
		constexpr char ColorMetatable[] = "mwse.openmw.Color";
		constexpr char AsyncCallbackMetatable[] = "mwse.openmw.AsyncCallback";
		constexpr char TimerCallbackMetatable[] = "mwse.openmw.TimerCallback";
		constexpr char StorageSectionMetatable[] = "mwse.openmw.StorageSection";
		constexpr std::size_t MaxFoundationNameLength = 256;

		struct Vector2Value { double x; double y; };
		struct ColorValue { double r; double g; double b; double a; };
		struct AsyncCallbackHandle { Host* host; std::uint32_t instanceId; int functionReference; };
		struct TimerCallbackHandle { Host* host; std::uint32_t instanceId; char name[MaxFoundationNameLength + 1]; };
		struct StorageSectionHandle {
			Host* host;
			std::uint32_t instanceId;
			bool player;
			bool readOnly;
			char name[MaxFoundationNameLength + 1];
		};

		StorageSectionHandle* checkStorageSection(lua_State* state) {
			return static_cast<StorageSectionHandle*>(luaL_checkudata(state,1,StorageSectionMetatable));
		}

		int absoluteIndex(lua_State* state, int index) {
			return index < 0 && index > LUA_REGISTRYINDEX ? lua_gettop(state) + index + 1 : index;
		}

		void copyBoundedName(lua_State* state, int index, char* destination, std::string_view kind) {
			size_t size = 0;
			const char* value = luaL_checklstring(state, index, &size);
			if (size == 0 || size > MaxFoundationNameLength) luaL_error(state, "%s name must contain 1 to 256 bytes", kind.data());
			std::memcpy(destination, value, size);
			destination[size] = '\0';
		}

		Vector2Value* checkVector2(lua_State* state, int index) {
			return static_cast<Vector2Value*>(luaL_checkudata(state, index, Vector2Metatable));
		}

		int pushVector2(lua_State* state, double x, double y) {
			auto* value = static_cast<Vector2Value*>(lua_newuserdata(state, sizeof(Vector2Value)));
			*value = { x, y };
			luaL_getmetatable(state, Vector2Metatable);
			lua_setmetatable(state, -2);
			return 1;
		}

		int vector2Create(lua_State* state) {
			return pushVector2(state, luaL_checknumber(state, 1), luaL_checknumber(state, 2));
		}

		int vector2Index(lua_State* state) {
			auto* value = checkVector2(state, 1);
			const char* key = luaL_checkstring(state, 2);
			if (std::strcmp(key, "x") == 0) { lua_pushnumber(state, value->x); return 1; }
			if (std::strcmp(key, "y") == 0) { lua_pushnumber(state, value->y); return 1; }
			luaL_getmetatable(state, Vector2Metatable);
			lua_getfield(state, -1, key);
			return 1;
		}

		int vector2Add(lua_State* state) { auto* a=checkVector2(state,1); auto* b=checkVector2(state,2); return pushVector2(state,a->x+b->x,a->y+b->y); }
		int vector2Subtract(lua_State* state) { auto* a=checkVector2(state,1); auto* b=checkVector2(state,2); return pushVector2(state,a->x-b->x,a->y-b->y); }
		int vector2Negate(lua_State* state) { auto* a=checkVector2(state,1); return pushVector2(state,-a->x,-a->y); }
		int vector2Multiply(lua_State* state) {
			if (lua_isnumber(state, 1)) { const double k=lua_tonumber(state,1); auto* v=checkVector2(state,2); return pushVector2(state,k*v->x,k*v->y); }
			auto* a=checkVector2(state,1);
			if (lua_isnumber(state,2)) { const double k=lua_tonumber(state,2); return pushVector2(state,a->x*k,a->y*k); }
			auto* b=checkVector2(state,2); lua_pushnumber(state,a->x*b->x+a->y*b->y); return 1;
		}
		int vector2Divide(lua_State* state) { auto* a=checkVector2(state,1); const double k=luaL_checknumber(state,2); return pushVector2(state,a->x/k,a->y/k); }
		int vector2Equal(lua_State* state) { auto* a=checkVector2(state,1); auto* b=checkVector2(state,2); lua_pushboolean(state,a->x==b->x&&a->y==b->y); return 1; }
		int vector2ToString(lua_State* state) { auto* v=checkVector2(state,1); std::ostringstream s; s<<'('<<v->x<<", "<<v->y<<')'; const auto text=s.str(); lua_pushlstring(state,text.data(),text.size()); return 1; }
		int vector2Length2(lua_State* state) { auto* v=checkVector2(state,1); lua_pushnumber(state,v->x*v->x+v->y*v->y); return 1; }
		int vector2Length(lua_State* state) { auto* v=checkVector2(state,1); lua_pushnumber(state,std::sqrt(v->x*v->x+v->y*v->y)); return 1; }
		int vector2Normalize(lua_State* state) { auto* v=checkVector2(state,1); const double l=std::sqrt(v->x*v->x+v->y*v->y); if(l==0.0)return luaL_error(state,"cannot normalize a zero-length Vector2"); pushVector2(state,v->x/l,v->y/l); lua_pushnumber(state,l); return 2; }
		int vector2Rotate(lua_State* state) { auto* v=checkVector2(state,1); const double a=luaL_checknumber(state,2); const double c=std::cos(a),s=std::sin(a); return pushVector2(state,v->x*c-v->y*s,v->x*s+v->y*c); }
		int vector2Dot(lua_State* state) { auto* a=checkVector2(state,1); auto* b=checkVector2(state,2); lua_pushnumber(state,a->x*b->x+a->y*b->y); return 1; }
		int vector2ElementMultiply(lua_State* state) { auto* a=checkVector2(state,1); auto* b=checkVector2(state,2); return pushVector2(state,a->x*b->x,a->y*b->y); }
		int vector2ElementDivide(lua_State* state) { auto* a=checkVector2(state,1); auto* b=checkVector2(state,2); return pushVector2(state,a->x/b->x,a->y/b->y); }

		ColorValue* checkColor(lua_State* state, int index) { return static_cast<ColorValue*>(luaL_checkudata(state,index,ColorMetatable)); }
		int pushColor(lua_State* state, double r, double g, double b, double a) {
			auto* value=static_cast<ColorValue*>(lua_newuserdata(state,sizeof(ColorValue))); *value={r,g,b,a};
			luaL_getmetatable(state,ColorMetatable); lua_setmetatable(state,-2); return 1;
		}
		int colorRgba(lua_State* state) { return pushColor(state,luaL_checknumber(state,1),luaL_checknumber(state,2),luaL_checknumber(state,3),luaL_checknumber(state,4)); }
		int colorRgb(lua_State* state) { return pushColor(state,luaL_checknumber(state,1),luaL_checknumber(state,2),luaL_checknumber(state,3),1.0); }
		int colorIndex(lua_State* state) {
			auto* c=checkColor(state,1); const char* key=luaL_checkstring(state,2);
			if(std::strcmp(key,"r")==0)lua_pushnumber(state,c->r); else if(std::strcmp(key,"g")==0)lua_pushnumber(state,c->g);
			else if(std::strcmp(key,"b")==0)lua_pushnumber(state,c->b); else if(std::strcmp(key,"a")==0)lua_pushnumber(state,c->a);
			else { luaL_getmetatable(state,ColorMetatable); lua_getfield(state,-1,key); }
			return 1;
		}
		int colorEqual(lua_State* state) { auto*a=checkColor(state,1);auto*b=checkColor(state,2);lua_pushboolean(state,a->r==b->r&&a->g==b->g&&a->b==b->b&&a->a==b->a);return 1; }
		int colorAsHex(lua_State* state) { auto*c=checkColor(state,1); auto cv=[](double v){return std::clamp(static_cast<int>(std::lround(v*255.0)),0,255);}; char text[7]; std::snprintf(text,sizeof(text),"%02x%02x%02x",cv(c->r),cv(c->g),cv(c->b)); lua_pushstring(state,text); return 1; }
		int colorAsRgb(lua_State* state) { auto*c=checkColor(state,1); lua_newtable(state); lua_pushnumber(state,c->r);lua_setfield(state,-2,"x");lua_pushnumber(state,c->g);lua_setfield(state,-2,"y");lua_pushnumber(state,c->b);lua_setfield(state,-2,"z");return 1; }
		int colorAsRgba(lua_State* state) { auto*c=checkColor(state,1); lua_newtable(state); lua_pushnumber(state,c->r);lua_setfield(state,-2,"x");lua_pushnumber(state,c->g);lua_setfield(state,-2,"y");lua_pushnumber(state,c->b);lua_setfield(state,-2,"z");lua_pushnumber(state,c->a);lua_setfield(state,-2,"w");return 1; }

		int utilRound(lua_State* state) { const double v=luaL_checknumber(state,1); lua_pushnumber(state,v>=0.0?std::floor(v+0.5):std::ceil(v-0.5)); return 1; }
		int utilRemap(lua_State* state) { const double v=luaL_checknumber(state,1),a=luaL_checknumber(state,2),b=luaL_checknumber(state,3),c=luaL_checknumber(state,4),d=luaL_checknumber(state,5); lua_pushnumber(state,c+(v-a)*(d-c)/(b-a)); return 1; }
		int utilClamp(lua_State* state) { const double v=luaL_checknumber(state,1),a=luaL_checknumber(state,2),b=luaL_checknumber(state,3); lua_pushnumber(state,std::clamp(v,a,b)); return 1; }
		int utilNormalizeAngle(lua_State* state) { constexpr double Pi=3.14159265358979323846; const double v=luaL_checknumber(state,1); const double turns=v/(2.0*Pi)+0.5; lua_pushnumber(state,(turns-std::floor(turns)-0.5)*(2.0*Pi)); return 1; }
		int immutableNewIndex(lua_State* state) { return luaL_error(state,"attempt to modify a read-only OpenMW value"); }

		void ensureFoundationMetatables(lua_State* state) {
			if (luaL_newmetatable(state, Vector2Metatable)) {
				lua_pushcfunction(state,vector2Index);lua_setfield(state,-2,"__index");lua_pushcfunction(state,immutableNewIndex);lua_setfield(state,-2,"__newindex");
				lua_pushcfunction(state,vector2Add);lua_setfield(state,-2,"__add");lua_pushcfunction(state,vector2Subtract);lua_setfield(state,-2,"__sub");lua_pushcfunction(state,vector2Negate);lua_setfield(state,-2,"__unm");
				lua_pushcfunction(state,vector2Multiply);lua_setfield(state,-2,"__mul");lua_pushcfunction(state,vector2Divide);lua_setfield(state,-2,"__div");lua_pushcfunction(state,vector2Equal);lua_setfield(state,-2,"__eq");lua_pushcfunction(state,vector2ToString);lua_setfield(state,-2,"__tostring");
				lua_pushcfunction(state,vector2Length);lua_setfield(state,-2,"length");lua_pushcfunction(state,vector2Length2);lua_setfield(state,-2,"length2");lua_pushcfunction(state,vector2Normalize);lua_setfield(state,-2,"normalize");lua_pushcfunction(state,vector2Rotate);lua_setfield(state,-2,"rotate");lua_pushcfunction(state,vector2Dot);lua_setfield(state,-2,"dot");lua_pushcfunction(state,vector2ElementMultiply);lua_setfield(state,-2,"emul");lua_pushcfunction(state,vector2ElementDivide);lua_setfield(state,-2,"ediv");
				lua_pushboolean(state,0);lua_setfield(state,-2,"__metatable");
			} lua_pop(state,1);
			if (luaL_newmetatable(state, ColorMetatable)) {
				lua_pushcfunction(state,colorIndex);lua_setfield(state,-2,"__index");lua_pushcfunction(state,immutableNewIndex);lua_setfield(state,-2,"__newindex");lua_pushcfunction(state,colorEqual);lua_setfield(state,-2,"__eq");
				lua_pushcfunction(state,colorAsHex);lua_setfield(state,-2,"asHex");lua_pushcfunction(state,colorAsRgb);lua_setfield(state,-2,"asRgb");lua_pushcfunction(state,colorAsRgba);lua_setfield(state,-2,"asRgba");lua_pushboolean(state,0);lua_setfield(state,-2,"__metatable");
			} lua_pop(state,1);
		}

		void cloneValue(lua_State* state, int index, std::set<const void*>& visited, int depth = 0) {
			if (depth > 64) luaL_error(state, "storage value exceeds the maximum table depth");
			index = absoluteIndex(state, index);
			switch (lua_type(state,index)) {
			case LUA_TNIL: lua_pushnil(state); return;
			case LUA_TBOOLEAN: lua_pushboolean(state,lua_toboolean(state,index)); return;
			case LUA_TNUMBER: lua_pushnumber(state,lua_tonumber(state,index)); return;
			case LUA_TSTRING: { size_t size=0;const char* text=lua_tolstring(state,index,&size);lua_pushlstring(state,text,size);return; }
			case LUA_TTABLE: {
				const void* identity=lua_topointer(state,index);
				if(!visited.insert(identity).second)luaL_error(state,"storage values may not contain cycles or aliased tables");
				lua_newtable(state); const int destination=lua_gettop(state); lua_pushnil(state);
				while(lua_next(state,index)!=0) {
					const int keyType=lua_type(state,-2);
					if(keyType!=LUA_TBOOLEAN&&keyType!=LUA_TNUMBER&&keyType!=LUA_TSTRING)luaL_error(state,"storage table keys must be booleans, numbers, or strings");
					cloneValue(state,-2,visited,depth+1); cloneValue(state,-2,visited,depth+1); lua_settable(state,destination); lua_pop(state,1);
				}
				return;
			}
			default: luaL_error(state,"unsupported storage value type '%s'",lua_typename(state,lua_type(state,index)));
			}
		}
	}

	int Host::readOnlyNewIndexThunk(lua_State* state) {
		return luaL_error(state, "attempt to modify a read-only OpenMW value");
	}

	int Host::strictReadOnlyIndexThunk(lua_State* state) {
		lua_pushvalue(state, 2);
		lua_rawget(state, lua_upvalueindex(1));
		if (lua_isnil(state, -1)) return luaL_error(state, "key is not present in strict read-only table");
		return 1;
	}

	void Host::pushReadOnlyProxy(int valueIndex, bool strict) {
		valueIndex = absoluteIndex(mLua, valueIndex);
		lua_newtable(mLua);
		lua_newtable(mLua);
		if (strict) {
			lua_pushvalue(mLua, valueIndex);
			lua_pushcclosure(mLua, &strictReadOnlyIndexThunk, 1);
		}
		else lua_pushvalue(mLua, valueIndex);
		lua_setfield(mLua, -2, "__index");
		lua_pushcfunction(mLua, &readOnlyNewIndexThunk); lua_setfield(mLua, -2, "__newindex");
		lua_pushboolean(mLua, 0); lua_setfield(mLua, -2, "__metatable");
		lua_setmetatable(mLua, -2);
	}

	void Host::pushUtilPackage(ScriptInstance&) {
		ensureFoundationMetatables(mLua);
		lua_newtable(mLua);
		lua_pushcfunction(mLua,utilRound);lua_setfield(mLua,-2,"round");
		lua_pushcfunction(mLua,utilRemap);lua_setfield(mLua,-2,"remap");
		lua_pushcfunction(mLua,utilClamp);lua_setfield(mLua,-2,"clamp");
		lua_pushcfunction(mLua,utilNormalizeAngle);lua_setfield(mLua,-2,"normalizeAngle");
		lua_pushcfunction(mLua,vector2Create);lua_setfield(mLua,-2,"vector2");
		lua_newtable(mLua); lua_pushcfunction(mLua,colorRgba);lua_setfield(mLua,-2,"rgba");lua_pushcfunction(mLua,colorRgb);lua_setfield(mLua,-2,"rgb");
		pushReadOnlyProxy(-1,false); lua_remove(mLua,-2); lua_setfield(mLua,-2,"color");
		pushReadOnlyProxy(-1,false); lua_remove(mLua,-2);
	}

	Host::ScriptInstance* Host::findInstance(std::uint32_t id) const {
		for (const auto& instance : mInstances) if (instance->id == id) return instance.get();
		return nullptr;
	}

	int Host::interfaceIndexThunk(lua_State* state) {
		auto* host=static_cast<Host*>(lua_touserdata(state,lua_upvalueindex(1)));
		const auto id=static_cast<std::uint32_t>(lua_tointeger(state,lua_upvalueindex(2)));
		if(host==nullptr||!lua_isstring(state,2))return luaL_error(state,"invalid openmw.interfaces lookup");
		auto* requester=host->findInstance(id); if(requester==nullptr)return luaL_error(state,"invalid script environment handle");
		size_t size=0;const char* key=lua_tolstring(state,2,&size); ScriptInstance* selected=nullptr;
		for(const auto& candidate:host->mInstances) {
			if(candidate->container!=requester->container||candidate->interfaceReference==LUA_NOREF||candidate->interfaceName.size()!=size||std::memcmp(candidate->interfaceName.data(),key,size)!=0)continue;
			if(selected==nullptr||candidate->definitionOrder>selected->definitionOrder)selected=candidate.get();
		}
		++host->mInterfaceLookups;
		if(selected==nullptr)lua_pushnil(state);else lua_rawgeti(state,LUA_REGISTRYINDEX,selected->interfaceReference);
		return 1;
	}

	void Host::pushInterfacesPackage(ScriptInstance& instance) {
		lua_newtable(mLua); lua_newtable(mLua);
		lua_pushlightuserdata(mLua,this);lua_pushinteger(mLua,instance.id);lua_pushcclosure(mLua,&interfaceIndexThunk,2);lua_setfield(mLua,-2,"__index");
		lua_pushcfunction(mLua,&readOnlyNewIndexThunk);lua_setfield(mLua,-2,"__newindex");lua_pushboolean(mLua,0);lua_setfield(mLua,-2,"__metatable");lua_setmetatable(mLua,-2);
	}

	void Host::recordFoundationProbe(std::string_view name, bool passed) {
		if(name.empty()||name.size()>MaxFoundationNameLength)throw std::runtime_error("foundation probe name must contain 1 to 256 bytes");
		auto [it,inserted]=mFoundationProbes.emplace(std::string(name),passed);
		if(!inserted)it->second=it->second&&passed;
		if(!passed)throw std::runtime_error("foundation probe failed: "+std::string(name));
	}

	int Host::compatibilityRecordProbeThunk(lua_State* state) {
		auto* host=static_cast<Host*>(lua_touserdata(state,lua_upvalueindex(1)));
		if(host==nullptr||(host->mConfig.flags&InitializationHarnessMode)==0)return luaL_error(state,"foundation probes are available only in harness mode");
		size_t size=0;const char* name=luaL_checklstring(state,1,&size);const bool passed=lua_toboolean(state,2)!=0;
		try{host->recordFoundationProbe(std::string_view(name,size),passed);lua_pushboolean(state,1);return 1;}catch(const std::exception& error){return luaL_error(state,"%s",error.what());}
	}

	int Host::asyncRegisterTimerCallbackThunk(lua_State* state) {
		auto* host=static_cast<Host*>(lua_touserdata(state,lua_upvalueindex(1)));const auto id=static_cast<std::uint32_t>(lua_tointeger(state,lua_upvalueindex(2)));
		auto* instance=host?host->findInstance(id):nullptr;if(instance==nullptr)return luaL_error(state,"invalid async package handle");
		char name[MaxFoundationNameLength+1];copyBoundedName(state,2,name,"timer callback");luaL_checktype(state,3,LUA_TFUNCTION);
		lua_pushvalue(state,3);const int reference=luaL_ref(state,LUA_REGISTRYINDEX);
		if(auto it=instance->timerCallbacks.find(name);it!=instance->timerCallbacks.end()){luaL_unref(state,LUA_REGISTRYINDEX,it->second);it->second=reference;}else instance->timerCallbacks.emplace(name,reference);
		auto* callback=static_cast<TimerCallbackHandle*>(lua_newuserdata(state,sizeof(TimerCallbackHandle)));callback->host=host;callback->instanceId=id;std::strcpy(callback->name,name);
		luaL_getmetatable(state,TimerCallbackMetatable);lua_setmetatable(state,-2);return 1;
	}

	int Host::asyncCallbackCallThunk(lua_State* state) {
		auto* callback=static_cast<AsyncCallbackHandle*>(luaL_checkudata(state,1,AsyncCallbackMetatable));
		if(callback->host==nullptr||callback->functionReference==LUA_NOREF||callback->host->findInstance(callback->instanceId)==nullptr){lua_pushnil(state);return 1;}
		const int arguments=lua_gettop(state)-1;lua_rawgeti(state,LUA_REGISTRYINDEX,callback->functionReference);lua_insert(state,1);lua_remove(state,2);
		if(lua_pcall(state,arguments,LUA_MULTRET,0)!=0)return lua_error(state);return lua_gettop(state);
	}

	int Host::asyncCallbackThunk(lua_State* state) {
		auto* host=static_cast<Host*>(lua_touserdata(state,lua_upvalueindex(1)));const auto id=static_cast<std::uint32_t>(lua_tointeger(state,lua_upvalueindex(2)));
		if(host==nullptr||host->findInstance(id)==nullptr)return luaL_error(state,"invalid async package handle");luaL_checktype(state,2,LUA_TFUNCTION);
		lua_pushvalue(state,2);const int reference=luaL_ref(state,LUA_REGISTRYINDEX);
		auto* callback=static_cast<AsyncCallbackHandle*>(lua_newuserdata(state,sizeof(AsyncCallbackHandle)));*callback={host,id,reference};
		luaL_getmetatable(state,AsyncCallbackMetatable);lua_setmetatable(state,-2);return 1;
	}

	int Host::asyncNewTimerThunk(lua_State* state) {
		auto* host=static_cast<Host*>(lua_touserdata(state,lua_upvalueindex(1)));const auto id=static_cast<std::uint32_t>(lua_tointeger(state,lua_upvalueindex(2)));
		const bool game=lua_toboolean(state,lua_upvalueindex(3))!=0,serializable=lua_toboolean(state,lua_upvalueindex(4))!=0;
		auto* instance=host?host->findInstance(id):nullptr;if(instance==nullptr)return luaL_error(state,"invalid async package handle");const double delay=luaL_checknumber(state,2);if(!std::isfinite(delay)||delay<0.0)return luaL_error(state,"timer delay must be a finite non-negative number");
		Timer timer{};timer.sequence=host->mNextTimerSequence++;timer.instanceId=id;timer.gameTime=game;timer.serializable=serializable;timer.dueTime=(game?host->mGameTimeSeconds:host->mSimulationTimeSeconds)+delay;
		if(serializable){auto* callback=static_cast<TimerCallbackHandle*>(luaL_checkudata(state,3,TimerCallbackMetatable));if(callback->host!=host||callback->instanceId!=id)return luaL_error(state,"timer callback belongs to another script environment");timer.callbackName=callback->name;lua_pushvalue(state,4);timer.argumentReference=luaL_ref(state,LUA_REGISTRYINDEX);}
		else{luaL_checktype(state,3,LUA_TFUNCTION);lua_pushvalue(state,3);timer.functionReference=luaL_ref(state,LUA_REGISTRYINDEX);}
		host->mTimers.push_back(std::move(timer));++host->mTimersScheduled;return 0;
	}

	void Host::pushAsyncPackage(ScriptInstance& instance) {
		if(luaL_newmetatable(mLua,AsyncCallbackMetatable)){lua_pushcfunction(mLua,&asyncCallbackCallThunk);lua_setfield(mLua,-2,"__call");lua_pushstring(mLua,"Callback");lua_setfield(mLua,-2,"__name");lua_pushboolean(mLua,0);lua_setfield(mLua,-2,"__metatable");}lua_pop(mLua,1);
		if(luaL_newmetatable(mLua,TimerCallbackMetatable)){lua_pushboolean(mLua,0);lua_setfield(mLua,-2,"__metatable");}lua_pop(mLua,1);
		lua_newtable(mLua);
		auto method=[&](const char* name,lua_CFunction function){lua_pushlightuserdata(mLua,this);lua_pushinteger(mLua,instance.id);lua_pushcclosure(mLua,function,2);lua_setfield(mLua,-2,name);};
		method("registerTimerCallback",&asyncRegisterTimerCallbackThunk);method("callback",&asyncCallbackThunk);
		auto timer=[&](const char* name,bool game,bool serializable){lua_pushlightuserdata(mLua,this);lua_pushinteger(mLua,instance.id);lua_pushboolean(mLua,game);lua_pushboolean(mLua,serializable);lua_pushcclosure(mLua,&asyncNewTimerThunk,4);lua_setfield(mLua,-2,name);};
		timer("newSimulationTimer",false,true);timer("newGameTimer",true,true);timer("newUnsavableSimulationTimer",false,false);timer("newUnsavableGameTimer",true,false);
		pushReadOnlyProxy(-1,false);lua_remove(mLua,-2);
	}

	void Host::processTimers() {
		std::vector<Timer> current;current.swap(mTimers);
		std::stable_sort(current.begin(),current.end(),[](const Timer& a,const Timer& b){if(a.dueTime!=b.dueTime)return a.dueTime<b.dueTime;return a.sequence<b.sequence;});
		for(auto& timer:current) {
			const double now=timer.gameTime?mGameTimeSeconds:mSimulationTimeSeconds;
			if(timer.dueTime>now){mTimers.push_back(std::move(timer));continue;}
			auto* instance=findInstance(timer.instanceId);if(instance==nullptr)continue;int functionReference=timer.functionReference;
			if(timer.serializable){auto it=instance->timerCallbacks.find(timer.callbackName);if(it==instance->timerCallbacks.end()){log(LogSeverity::Error,"timer","registered timer callback is no longer available: "+timer.callbackName,instance);continue;}functionReference=it->second;}
			lua_rawgeti(mLua,LUA_REGISTRYINDEX,functionReference);int arguments=0;if(timer.serializable){lua_rawgeti(mLua,LUA_REGISTRYINDEX,timer.argumentReference);arguments=1;}
			if(lua_pcall(mLua,arguments,0,0)!=0){std::string message=lua_tostring(mLua,-1);lua_pop(mLua,1);const std::string diagnostic=instance->contentFile+"|"+instance->container+"|"+instance->scriptPath+"|timer|"+message;if(mDiagnostics.size()<128)mDiagnostics.push_back(diagnostic);log(LogSeverity::Error,"timer",diagnostic,instance);}else ++mTimersFired;
			if(timer.functionReference!=LUA_NOREF)luaL_unref(mLua,LUA_REGISTRYINDEX,timer.functionReference);if(timer.argumentReference!=LUA_NOREF)luaL_unref(mLua,LUA_REGISTRYINDEX,timer.argumentReference);
		}
	}

	Host::StorageSection& Host::getStorageSection(bool player, std::string_view name) {
		if(name.empty()||name.size()>MaxFoundationNameLength)throw std::runtime_error("storage section name must contain 1 to 256 bytes");
		auto& storage=player?mPlayerStorage:mGlobalStorage;return storage[std::string(name)];
	}

	void Host::pushStorageSection(ScriptInstance& instance, bool player, std::string_view name, bool readOnly) {
		if(name.empty()||name.size()>MaxFoundationNameLength)throw std::runtime_error("storage section name must contain 1 to 256 bytes");
		getStorageSection(player,name);
		if(luaL_newmetatable(mLua,StorageSectionMetatable)) {
			lua_pushvalue(mLua,-1);lua_setfield(mLua,-2,"__index");lua_pushboolean(mLua,0);lua_setfield(mLua,-2,"__metatable");
			lua_pushcfunction(mLua,&storageGetThunk);lua_setfield(mLua,-2,"get");lua_pushcfunction(mLua,&storageGetCopyThunk);lua_setfield(mLua,-2,"getCopy");lua_pushcfunction(mLua,&storageAsTableThunk);lua_setfield(mLua,-2,"asTable");
			lua_pushcfunction(mLua,&storageSubscribeThunk);lua_setfield(mLua,-2,"subscribe");lua_pushcfunction(mLua,&storageSetThunk);lua_setfield(mLua,-2,"set");lua_pushcfunction(mLua,&storageResetThunk);lua_setfield(mLua,-2,"reset");
			lua_pushcfunction(mLua,&storageSetLifeTimeThunk);lua_setfield(mLua,-2,"removeOnExit");lua_pushcfunction(mLua,&storageSetLifeTimeThunk);lua_setfield(mLua,-2,"setLifeTime");
		}lua_pop(mLua,1);
		auto* handle=static_cast<StorageSectionHandle*>(lua_newuserdata(mLua,sizeof(StorageSectionHandle)));handle->host=this;handle->instanceId=instance.id;handle->player=player;handle->readOnly=readOnly;std::memcpy(handle->name,name.data(),name.size());handle->name[name.size()]='\0';
		luaL_getmetatable(mLua,StorageSectionMetatable);lua_setmetatable(mLua,-2);
	}

	void Host::notifyStorage(StorageSection& section, std::string_view sectionName, std::optional<std::string_view> key) {
		section.notifying=true;
		for(const auto& subscription:section.subscriptions) {
			auto* instance=findInstance(subscription.instanceId);if(instance==nullptr)continue;lua_rawgeti(mLua,LUA_REGISTRYINDEX,subscription.callbackReference);lua_pushlstring(mLua,sectionName.data(),sectionName.size());if(key)lua_pushlstring(mLua,key->data(),key->size());else lua_pushnil(mLua);
			if(lua_pcall(mLua,2,0,0)!=0){std::string message=lua_tostring(mLua,-1);lua_pop(mLua,1);const std::string diagnostic=instance->contentFile+"|"+instance->container+"|"+instance->scriptPath+"|storage.subscribe|"+message;if(mDiagnostics.size()<128)mDiagnostics.push_back(diagnostic);log(LogSeverity::Error,"storage",diagnostic,instance);}
			else ++mStorageNotifications;
		}
		section.notifying=false;
	}

	int Host::storageSectionThunk(lua_State* state) {
		auto* host=static_cast<Host*>(lua_touserdata(state,lua_upvalueindex(1)));const auto id=static_cast<std::uint32_t>(lua_tointeger(state,lua_upvalueindex(2)));const bool player=lua_toboolean(state,lua_upvalueindex(3))!=0;
		auto* instance=host?host->findInstance(id):nullptr;if(instance==nullptr)return luaL_error(state,"invalid storage package handle");size_t size=0;const char* name=luaL_checklstring(state,1,&size);
		const bool readOnly=player?!(instance->container=="PLAYER"||instance->container=="MENU"):instance->container!="GLOBAL";
		try{host->pushStorageSection(*instance,player,std::string_view(name,size),readOnly);return 1;}catch(const std::exception& error){return luaL_error(state,"%s",error.what());}
	}

	int Host::storageAllSectionsThunk(lua_State* state) {
		auto* host=static_cast<Host*>(lua_touserdata(state,lua_upvalueindex(1)));const auto id=static_cast<std::uint32_t>(lua_tointeger(state,lua_upvalueindex(2)));const bool player=lua_toboolean(state,lua_upvalueindex(3))!=0;
		auto* instance=host?host->findInstance(id):nullptr;if(instance==nullptr)return luaL_error(state,"invalid storage package handle");lua_newtable(state);auto& storage=player?host->mPlayerStorage:host->mGlobalStorage;
		const bool readOnly=player?!(instance->container=="PLAYER"||instance->container=="MENU"):instance->container!="GLOBAL";
		for(const auto& [name,_]:storage){host->pushStorageSection(*instance,player,name,readOnly);lua_setfield(state,-2,name.c_str());}return 1;
	}

	int Host::storageGetThunk(lua_State* state) {
		auto* handle=checkStorageSection(state);if(handle->host==nullptr||handle->host->findInstance(handle->instanceId)==nullptr)return luaL_error(state,"invalid storage section handle");size_t size=0;const char* key=luaL_checklstring(state,2,&size);auto& section=handle->host->getStorageSection(handle->player,handle->name);auto it=section.values.find(std::string(key,size));if(it==section.values.end()){lua_pushnil(state);return 1;}lua_rawgeti(state,LUA_REGISTRYINDEX,it->second);
		if(lua_istable(state,-1)){std::set<const void*> visited;cloneValue(state,-1,visited);lua_remove(state,-2);handle->host->pushReadOnlyProxy(-1,false);lua_remove(state,-2);}return 1;
	}

	int Host::storageGetCopyThunk(lua_State* state) {
		auto* handle=checkStorageSection(state);if(handle->host==nullptr||handle->host->findInstance(handle->instanceId)==nullptr)return luaL_error(state,"invalid storage section handle");size_t size=0;const char* key=luaL_checklstring(state,2,&size);auto& section=handle->host->getStorageSection(handle->player,handle->name);auto it=section.values.find(std::string(key,size));if(it==section.values.end()){lua_pushnil(state);return 1;}lua_rawgeti(state,LUA_REGISTRYINDEX,it->second);std::set<const void*> visited;cloneValue(state,-1,visited);lua_remove(state,-2);return 1;
	}

	int Host::storageAsTableThunk(lua_State* state) {
		auto* handle=checkStorageSection(state);if(handle->host==nullptr||handle->host->findInstance(handle->instanceId)==nullptr)return luaL_error(state,"invalid storage section handle");auto& section=handle->host->getStorageSection(handle->player,handle->name);lua_newtable(state);const int result=lua_gettop(state);
		for(const auto& [key,reference]:section.values){lua_rawgeti(state,LUA_REGISTRYINDEX,reference);std::set<const void*> visited;cloneValue(state,-1,visited);lua_remove(state,-2);lua_setfield(state,result,key.c_str());}return 1;
	}

	int Host::storageSubscribeThunk(lua_State* state) {
		auto* handle=checkStorageSection(state);auto* callback=static_cast<AsyncCallbackHandle*>(luaL_checkudata(state,2,AsyncCallbackMetatable));if(handle->host==nullptr||callback->host!=handle->host||callback->instanceId!=handle->instanceId)return luaL_error(state,"storage subscription callback belongs to another script environment");auto& section=handle->host->getStorageSection(handle->player,handle->name);lua_pushvalue(state,2);section.subscriptions.push_back({handle->instanceId,luaL_ref(state,LUA_REGISTRYINDEX)});return 0;
	}

	int Host::storageSetThunk(lua_State* state) {
		auto* handle=checkStorageSection(state);if(handle->readOnly)return luaL_error(state,"access to storage is read only");size_t size=0;const char* key=luaL_checklstring(state,2,&size);if(size==0||size>MaxFoundationNameLength)return luaL_error(state,"storage key must contain 1 to 256 bytes");auto& section=handle->host->getStorageSection(handle->player,handle->name);if(section.notifying)return luaL_error(state,"storage handler must not change the section it handles");const std::string keyString(key,size);auto old=section.values.find(keyString);
		if(lua_isnil(state,3)){if(old!=section.values.end()){luaL_unref(state,LUA_REGISTRYINDEX,old->second);section.values.erase(old);}}
		else{std::set<const void*> visited;cloneValue(state,3,visited);const int reference=luaL_ref(state,LUA_REGISTRYINDEX);if(old!=section.values.end()){luaL_unref(state,LUA_REGISTRYINDEX,old->second);old->second=reference;}else section.values.emplace(keyString,reference);}
		handle->host->notifyStorage(section,handle->name,keyString);return 0;
	}

	int Host::storageResetThunk(lua_State* state) {
		auto* handle=checkStorageSection(state);if(handle->readOnly)return luaL_error(state,"access to storage is read only");auto& section=handle->host->getStorageSection(handle->player,handle->name);if(section.notifying)return luaL_error(state,"storage handler must not change the section it handles");std::map<std::string,int> replacement;
		if(!lua_isnoneornil(state,2)){luaL_checktype(state,2,LUA_TTABLE);lua_pushnil(state);while(lua_next(state,2)!=0){size_t size=0;const char* key=luaL_checklstring(state,-2,&size);std::set<const void*> visited;cloneValue(state,-1,visited);replacement.emplace(std::string(key,size),luaL_ref(state,LUA_REGISTRYINDEX));lua_pop(state,1);}}
		for(const auto& [_,reference]:section.values)luaL_unref(state,LUA_REGISTRYINDEX,reference);section.values=std::move(replacement);handle->host->notifyStorage(section,handle->name,std::nullopt);return 0;
	}

	int Host::storageSetLifeTimeThunk(lua_State* state) {
		auto* handle=checkStorageSection(state);if(handle->readOnly)return luaL_error(state,"access to storage is read only");const int lifeTime=lua_gettop(state)==1?2:static_cast<int>(luaL_checkinteger(state,2));if(lifeTime<0||lifeTime>2)return luaL_error(state,"invalid storage lifetime");handle->host->getStorageSection(handle->player,handle->name).lifeTime=static_cast<std::uint32_t>(lifeTime);return 0;
	}

	void Host::pushStoragePackage(ScriptInstance& instance) {
		lua_newtable(mLua);const int package=lua_gettop(mLua);
		lua_newtable(mLua);lua_pushinteger(mLua,0);lua_setfield(mLua,-2,"Persistent");lua_pushinteger(mLua,1);lua_setfield(mLua,-2,"GameSession");lua_pushinteger(mLua,2);lua_setfield(mLua,-2,"Temporary");pushReadOnlyProxy(-1,true);lua_remove(mLua,-2);lua_setfield(mLua,package,"LIFE_TIME");
		auto section=[&](const char* name,bool player){lua_pushlightuserdata(mLua,this);lua_pushinteger(mLua,instance.id);lua_pushboolean(mLua,player);lua_pushcclosure(mLua,&storageSectionThunk,3);lua_setfield(mLua,package,name);};
		auto all=[&](const char* name,bool player){lua_pushlightuserdata(mLua,this);lua_pushinteger(mLua,instance.id);lua_pushboolean(mLua,player);lua_pushcclosure(mLua,&storageAllSectionsThunk,3);lua_setfield(mLua,package,name);};
		section("globalSection",false);if(instance.container=="GLOBAL")all("allGlobalSections",false);if(instance.container=="PLAYER"||instance.container=="MENU"){section("playerSection",true);all("allPlayerSections",true);}
		pushReadOnlyProxy(package,false);lua_remove(mLua,package);
	}

	int Host::coreTimeThunk(lua_State* state) {
		auto* host=static_cast<Host*>(lua_touserdata(state,lua_upvalueindex(1)));const int selector=static_cast<int>(lua_tointeger(state,lua_upvalueindex(2)));if(host==nullptr)return luaL_error(state,"invalid openmw.core package handle");
		switch(selector){case 0:lua_pushnumber(state,host->mSimulationTimeSeconds);break;case 1:lua_pushnumber(state,host->mSimulationTimeScale);break;case 2:lua_pushnumber(state,host->mGameTimeSeconds);break;case 3:lua_pushnumber(state,host->mGameTimeScale);break;case 4:lua_pushboolean(state,host->mWorldPaused);break;case 5:lua_pushnumber(state,host->mRealTimeSeconds);break;case 6:lua_pushnumber(state,host->mRealFrameDuration);break;default:return luaL_error(state,"invalid core time selector");}return 1;
	}

	int Host::coreGetGameSettingThunk(lua_State* state) {
		auto* host=static_cast<Host*>(lua_touserdata(state,lua_upvalueindex(1)));if(host==nullptr||host->mCallbacks.getGameSetting==nullptr)return luaL_error(state,"openmw.core.getGMST is unavailable: the native bridge has no GMST callback");size_t size=0;const char* name=luaL_checklstring(state,1,&size);if(size==0||size>MaxBridgeStringLength)return luaL_error(state,"GMST name exceeds bridge bounds");
		BridgeValue value{sizeof(BridgeValue),BridgeAbiVersion,ValueType::None,0,0.0,{}};const Status status=host->mCallbacks.getGameSetting(host->mCallbacks.gameUserData,{name,static_cast<std::uint32_t>(size)},&value);if(status!=Status::Ok)return luaL_error(state,"native GMST lookup failed with bridge status %u",static_cast<unsigned>(status));if(value.structureSize!=sizeof(BridgeValue)||value.abiVersion!=BridgeAbiVersion)return luaL_error(state,"native GMST lookup returned an incompatible value structure");
		switch(value.type){case ValueType::None:lua_pushnil(state);return 1;case ValueType::Number:lua_pushnumber(state,value.number);return 1;case ValueType::Boolean:lua_pushboolean(state,value.number!=0.0);return 1;case ValueType::String:if(value.string.size>MaxBridgeStringLength||(value.string.size!=0&&value.string.data==nullptr))return luaL_error(state,"native GMST lookup returned an invalid string");lua_pushlstring(state,value.string.data,value.string.size);return 1;default:return luaL_error(state,"native GMST lookup returned an unknown value type");}
	}

	int Host::coreSendGlobalEventThunk(lua_State* state) {
		auto* host=static_cast<Host*>(lua_touserdata(state,lua_upvalueindex(1)));const auto id=static_cast<std::uint32_t>(lua_tointeger(state,lua_upvalueindex(2)));auto* instance=host?host->findInstance(id):nullptr;if(instance==nullptr)return luaL_error(state,"invalid openmw.core package handle");size_t size=0;const char* name=luaL_checklstring(state,1,&size);if(size==0||size>MaxFoundationNameLength)return luaL_error(state,"event name must contain 1 to 256 bytes");
		std::set<const void*> visited;cloneValue(state,2,visited);const int reference=luaL_ref(state,LUA_REGISTRYINDEX);DelayedEvent event{};event.sequence=host->mNextEventSequence++;event.name.assign(name,size);event.targetContainer="GLOBAL";event.payloadReference=reference;host->mNextEvents.push_back(std::move(event));return 0;
	}

	int Host::coreL10nFormatThunk(lua_State* state) {
		size_t size=0;const char* key=luaL_checklstring(state,1,&size);std::string result(key,size);
		if(lua_istable(state,2)){lua_pushnil(state);while(lua_next(state,2)!=0){if(lua_type(state,-2)==LUA_TSTRING){size_t keySize=0;const char* parameter=lua_tolstring(state,-2,&keySize);std::string replacement;switch(lua_type(state,-1)){case LUA_TSTRING:{size_t valueSize=0;const char* value=lua_tolstring(state,-1,&valueSize);replacement.assign(value,valueSize);break;}case LUA_TNUMBER:{std::ostringstream stream;stream<<lua_tonumber(state,-1);replacement=stream.str();break;}case LUA_TBOOLEAN:replacement=lua_toboolean(state,-1)?"true":"false";break;default:break;}const std::string token="{"+std::string(parameter,keySize)+"}";std::size_t position=0;while(!replacement.empty()&&(position=result.find(token,position))!=std::string::npos){result.replace(position,token.size(),replacement);position+=replacement.size();}}lua_pop(state,1);}}
		lua_pushlstring(state,result.data(),result.size());return 1;
	}

	int Host::coreL10nThunk(lua_State* state) {
		luaL_checktype(state,1,LUA_TSTRING);lua_pushcfunction(state,&coreL10nFormatThunk);return 1;
	}

	int Host::contentFilesIndexOfThunk(lua_State* state) {
		auto* host=static_cast<Host*>(lua_touserdata(state,lua_upvalueindex(1)));if(host==nullptr||host->mCallbacks.getContentFileCount==nullptr||host->mCallbacks.getContentFile==nullptr)return luaL_error(state,"content-file lookup is unavailable from the native bridge");size_t size=0;const char* wanted=luaL_checklstring(state,1,&size);const std::string wantedName(wanted,size);const auto count=host->mCallbacks.getContentFileCount(host->mCallbacks.gameUserData);
		for(std::uint32_t index=0;index<count;++index){StringView name{};if(host->mCallbacks.getContentFile(host->mCallbacks.gameUserData,index,&name)!=Status::Ok||name.size>MaxBridgeStringLength||(name.size!=0&&name.data==nullptr))return luaL_error(state,"native content-file enumeration failed");if(name.size==size&&_strnicmp(name.data,wantedName.c_str(),size)==0){lua_pushinteger(state,index+1);return 1;}}lua_pushnil(state);return 1;
	}

	int Host::contentFilesHasThunk(lua_State* state) { const int result=contentFilesIndexOfThunk(state);if(result!=1)return result;const bool present=!lua_isnil(state,-1);lua_pop(state,1);lua_pushboolean(state,present);return 1; }

	void Host::pushCorePackage(ScriptInstance& instance) {
		lua_newtable(mLua);const int package=lua_gettop(mLua);lua_pushinteger(mLua,OpenMWApiRevision);lua_setfield(mLua,package,"API_REVISION");
		auto time=[&](const char* name,int selector){lua_pushlightuserdata(mLua,this);lua_pushinteger(mLua,selector);lua_pushcclosure(mLua,&coreTimeThunk,2);lua_setfield(mLua,package,name);};
		time("getSimulationTime",0);time("getSimulationTimeScale",1);time("getGameTime",2);time("getGameTimeScale",3);time("isWorldPaused",4);time("getRealTime",5);if(instance.container!="GLOBAL")time("getRealFrameDuration",6);
		lua_pushlightuserdata(mLua,this);lua_pushcclosure(mLua,&coreGetGameSettingThunk,1);lua_setfield(mLua,package,"getGMST");lua_pushcfunction(mLua,&coreL10nThunk);lua_setfield(mLua,package,"l10n");
		lua_pushlightuserdata(mLua,this);lua_pushinteger(mLua,instance.id);lua_pushcclosure(mLua,&coreSendGlobalEventThunk,2);lua_setfield(mLua,package,"sendGlobalEvent");
		lua_newtable(mLua);const int content=lua_gettop(mLua);lua_newtable(mLua);const int list=lua_gettop(mLua);
		if(mCallbacks.getContentFileCount!=nullptr&&mCallbacks.getContentFile!=nullptr){const auto count=mCallbacks.getContentFileCount(mCallbacks.gameUserData);for(std::uint32_t index=0;index<count;++index){StringView name{};if(mCallbacks.getContentFile(mCallbacks.gameUserData,index,&name)!=Status::Ok||name.size>MaxBridgeStringLength||(name.size!=0&&name.data==nullptr))throw std::runtime_error("native content-file enumeration returned invalid data");std::string value(name.data,name.size);std::transform(value.begin(),value.end(),value.begin(),[](unsigned char c){return static_cast<char>(std::tolower(c));});lua_pushlstring(mLua,value.data(),value.size());lua_rawseti(mLua,list,index+1);}}
		pushReadOnlyProxy(list,false);lua_remove(mLua,list);lua_setfield(mLua,content,"list");lua_pushlightuserdata(mLua,this);lua_pushcclosure(mLua,&contentFilesIndexOfThunk,1);lua_setfield(mLua,content,"indexOf");lua_pushlightuserdata(mLua,this);lua_pushcclosure(mLua,&contentFilesHasThunk,1);lua_setfield(mLua,content,"has");pushReadOnlyProxy(content,false);lua_remove(mLua,content);lua_setfield(mLua,package,"contentFiles");
		pushReadOnlyProxy(package,false);lua_remove(mLua,package);
	}

	void Host::pushSelfPackage(ScriptInstance& instance) {
		if(instance.container!="PLAYER")throw std::runtime_error("openmw.self is available only in PLAYER and local script containers");lua_newtable(mLua);lua_pushboolean(mLua,1);lua_setfield(mLua,-2,"_mwseFoundationAvailable");lua_pushcfunction(mLua,&unsupportedThunk);lua_setfield(mLua,-2,"isActive");lua_pushcfunction(mLua,&unsupportedThunk);lua_setfield(mLua,-2,"enableAI");pushReadOnlyProxy(-1,false);lua_remove(mLua,-2);
	}

	void Host::pushBuiltinPackage(ScriptInstance& instance, std::string_view name) {
		if(name=="openmw.util")pushUtilPackage(instance);else if(name=="openmw.interfaces")pushInterfacesPackage(instance);else if(name=="openmw.async")pushAsyncPackage(instance);else if(name=="openmw.storage")pushStoragePackage(instance);else if(name=="openmw.core")pushCorePackage(instance);else if(name=="openmw.self")pushSelfPackage(instance);
		else if(name=="openmw.compatibility"){lua_newtable(mLua);lua_pushinteger(mLua,OpenMWApiRevision);lua_setfield(mLua,-2,"apiRevision");lua_pushinteger(mLua,BridgeVersion);lua_setfield(mLua,-2,"bridgeVersion");lua_pushboolean(mLua,1);lua_setfield(mLua,-2,"foundationPackagesAvailable");lua_pushboolean(mLua,0);lua_setfield(mLua,-2,"gameplayBindingsAvailable");lua_pushcfunction(mLua,&unsupportedThunk);lua_setfield(mLua,-2,"requireGameplayBinding");lua_pushlightuserdata(mLua,this);lua_pushcclosure(mLua,&compatibilityRecordProbeThunk,1);lua_setfield(mLua,-2,"recordFoundationProbe");pushReadOnlyProxy(-1,false);lua_remove(mLua,-2);}
		else throw std::runtime_error("unknown built-in compatibility package: "+std::string(name));
	}

}
