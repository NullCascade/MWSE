#include "OpenMWLuaHost.h"

namespace mwse::openmw::host {
	namespace {
		constexpr std::size_t MaxActionKeyLength = 256;
		constexpr const char* ObjectMetatable = "mwse.openmw.gameObject";
		constexpr const char* SkillIds[] = { "block", "armorer", "mediumarmor", "heavyarmor", "bluntweapon", "longblade", "axe", "spear", "athletics", "enchant", "destruction", "alteration", "illusion", "conjuration", "mysticism", "restoration", "alchemy", "unarmored", "security", "sneak", "acrobatics", "lightarmor", "shortblade", "marksman", "mercantile", "speechcraft", "handtohand" };

		struct ObjectUserdata { Host* host; OpaqueHandle handle; };

		std::string bridgeText(const BridgeText& value, std::string_view field) {
			if (value.size > BridgeTextCapacity) throw std::invalid_argument(std::string(field) + " exceeds bridge bounds");
			return std::string(value.data, value.size);
		}

		void cloneEventValue(lua_State* state, int index, std::set<const void*>& visited, int depth = 0) {
			if (depth > 32) luaL_error(state, "event payload nesting exceeds 32 levels");
			index = index > 0 ? index : lua_gettop(state) + index + 1;
			switch (lua_type(state, index)) {
			case LUA_TNIL: lua_pushnil(state); return;
			case LUA_TBOOLEAN: lua_pushboolean(state, lua_toboolean(state, index)); return;
			case LUA_TNUMBER: lua_pushnumber(state, lua_tonumber(state, index)); return;
			case LUA_TSTRING: { size_t size = 0; const char* value = lua_tolstring(state, index, &size); lua_pushlstring(state, value, size); return; }
			case LUA_TUSERDATA:
				if (lua_getmetatable(state, index)) {
					luaL_getmetatable(state, ObjectMetatable);
					const bool object = lua_rawequal(state, -1, -2) != 0;
					lua_pop(state, 2);
					if (object) { lua_pushvalue(state, index); return; }
				}
				luaL_error(state, "event payload contains unsupported userdata");
				return;
			case LUA_TTABLE: {
				const void* identity = lua_topointer(state, index);
				if (!visited.insert(identity).second) luaL_error(state, "event payload contains a cycle or shared table");
				lua_newtable(state);
				const int destination = lua_gettop(state);
				lua_pushnil(state);
				while (lua_next(state, index) != 0) {
					const int keyType = lua_type(state, -2);
					if (keyType != LUA_TSTRING && keyType != LUA_TNUMBER && keyType != LUA_TBOOLEAN) luaL_error(state, "event payload contains an unsupported table key");
					cloneEventValue(state, -2, visited, depth + 1);
					cloneEventValue(state, -2, visited, depth + 1);
					lua_settable(state, destination);
					lua_pop(state, 1);
				}
				visited.erase(identity);
				return;
			}
			default: luaL_error(state, "event payload contains an unsupported Lua value");
			}
		}

		bool isCallable(lua_State* state, int index) {
			if (lua_isfunction(state, index)) return true;
			if (!lua_getmetatable(state, index)) return false;
			lua_getfield(state, -1, "__call");
			const bool callable = lua_isfunction(state, -1);
			lua_pop(state, 2);
			return callable;
		}

		void pushBridgeEventTable(lua_State* state, std::string_view previous, std::string_view name) {
			lua_newtable(state);
			lua_pushlstring(state, previous.data(), previous.size()); lua_setfield(state, -2, "oldMode");
			lua_pushlstring(state, name.data(), name.size()); lua_setfield(state, -2, "newMode");
			lua_pushnil(state); lua_setfield(state, -2, "arg");
		}
	}

	Status Host::queueNativeEvent(const NativeEvent* eventData) {
		if (eventData == nullptr) return Status::InvalidArgument;
		if (eventData->structureSize != sizeof(NativeEvent)) return Status::StructureSizeMismatch;
		if (eventData->abiVersion != BridgeAbiVersion) return Status::AbiMismatch;
		if (mState != LifecycleState::Running) return Status::InvalidState;
		if (eventData->reserved != 0 || !std::isfinite(eventData->value)) return Status::InvalidArgument;
		if (eventData->type != NativeEventType::UiModeChanged && eventData->type != NativeEventType::PlayerDied
			&& eventData->type != NativeEventType::SkillLevelUp) return Status::Unsupported;
		if (eventData->type == NativeEventType::SkillLevelUp && eventData->index >= std::size(SkillIds)) return Status::OutOfRange;
		try {
			PendingNativeEvent pending{};
			pending.sequence = eventData->sequence == 0 ? mNextEventSequence++ : eventData->sequence;
			pending.type = eventData->type;
			pending.index = eventData->index;
			pending.value = eventData->value;
			pending.name = bridgeText(eventData->name, "native event name");
			pending.previous = bridgeText(eventData->previous, "native event previous value");
			pending.source = bridgeText(eventData->source, "native event source");
			mNextNativeEvents.push_back(std::move(pending));
			return Status::Ok;
		}
		catch (...) { return Status::InvalidArgument; }
	}

	Status Host::updateAction(const ActionUpdate* updateData) {
		if (updateData == nullptr) return Status::InvalidArgument;
		if (updateData->structureSize != sizeof(ActionUpdate)) return Status::StructureSizeMismatch;
		if (updateData->abiVersion != BridgeAbiVersion) return Status::AbiMismatch;
		if (mState != LifecycleState::Running) return Status::InvalidState;
		if (updateData->key.size == 0 || updateData->key.size > MaxActionKeyLength || updateData->key.data == nullptr
			|| !std::isfinite(updateData->value)) return Status::InvalidArgument;
		std::string key(updateData->key.data, updateData->key.size);
		auto action = mInputActions.find(key);
		if (action == mInputActions.end()) return Status::NotFound;
		if (action->second.type != updateData->type) return Status::WrongHandleType;
		if (updateData->type == ActionType::Boolean && updateData->value != 0.0 && updateData->value != 1.0) return Status::OutOfRange;
		if (updateData->type == ActionType::Range && (updateData->value < 0.0 || updateData->value > 1.0)) return Status::OutOfRange;
		mPendingActionUpdates.push_back({ std::move(key), updateData->type, updateData->value });
		return Status::Ok;
	}

	void Host::processActionUpdates() {
		std::vector<PendingActionUpdate> updates;
		updates.swap(mPendingActionUpdates);
		for (const auto& updateData : updates) {
			auto action = mInputActions.find(updateData.key);
			if (action == mInputActions.end() || action->second.type != updateData.type || action->second.value == updateData.value) continue;
			action->second.value = updateData.value;
			++mActionTransitions;
			for (auto handler = action->second.handlers.begin(); handler != action->second.handlers.end();) {
				auto* instance = findInstance(handler->instanceId);
				if (instance == nullptr) { handler = action->second.handlers.erase(handler); continue; }
				lua_rawgeti(mLua, LUA_REGISTRYINDEX, handler->reference);
				if (updateData.type == ActionType::Boolean) lua_pushboolean(mLua, updateData.value != 0.0); else lua_pushnumber(mLua, updateData.value);
				if (lua_pcall(mLua, 1, 0, 0) != 0) {
					std::string message = lua_tostring(mLua, -1); lua_pop(mLua, 1);
					const std::string diagnostic = instance->contentFile + "|" + instance->container + "|" + instance->scriptPath + "|input action " + updateData.key + "|" + message;
					if (mDiagnostics.size() < 128) mDiagnostics.push_back(diagnostic);
					log(LogSeverity::Error, "input", diagnostic, instance);
				}
				++handler;
			}
		}
	}

	void Host::callEventHandlersWithValue(std::string_view name, std::string_view targetContainer, int valueIndex) {
		valueIndex = valueIndex > 0 ? valueIndex : lua_gettop(mLua) + valueIndex + 1;
		for (std::size_t i = mInstances.size(); i > 0; --i) {
			auto& instance = *mInstances[i - 1];
			if (!targetContainer.empty() && instance.container != targetContainer) continue;
			auto handler = instance.eventHandlers.find(std::string(name));
			if (handler == instance.eventHandlers.end()) continue;
			if (mEventHandlerOrder.size() < 128) mEventHandlerOrder.push_back(instance.container + ":" + instance.scriptPath + ":" + std::string(name));
			lua_rawgeti(mLua, LUA_REGISTRYINDEX, handler->second);
			lua_pushvalue(mLua, valueIndex);
			if (lua_pcall(mLua, 1, 0, 0) != 0) {
				std::string message = lua_tostring(mLua, -1); lua_pop(mLua, 1);
				const std::string diagnostic = instance.contentFile + "|" + instance.container + "|" + instance.scriptPath + "|eventHandlers." + std::string(name) + "|" + message;
				if (mDiagnostics.size() < 128) mDiagnostics.push_back(diagnostic);
				log(LogSeverity::Error, "handler", diagnostic, &instance);
			}
		}
	}

	void Host::callSkillLevelUpHandlers(const PendingNativeEvent& eventData) {
		for (auto handler = mSkillLevelUpHandlers.begin(); handler != mSkillLevelUpHandlers.end();) {
			auto* instance = findInstance(handler->instanceId);
			if (instance == nullptr) { handler = mSkillLevelUpHandlers.erase(handler); continue; }
			lua_rawgeti(mLua, LUA_REGISTRYINDEX, handler->reference);
			lua_pushstring(mLua, SkillIds[eventData.index]);
			const std::string source = eventData.source == "progress" ? "usage" : eventData.source;
			lua_pushlstring(mLua, source.data(), source.size());
			lua_newtable(mLua);
			lua_pushnumber(mLua, eventData.value); lua_setfield(mLua, -2, "skillLevel");
			if (lua_pcall(mLua, 3, 1, 0) != 0) {
				std::string message = lua_tostring(mLua, -1); lua_pop(mLua, 1);
				const std::string diagnostic = instance->contentFile + "|" + instance->container + "|" + instance->scriptPath + "|SkillProgression.addSkillLevelUpHandler|" + message;
				if (mDiagnostics.size() < 128) mDiagnostics.push_back(diagnostic);
				log(LogSeverity::Error, "handler", diagnostic, instance);
			}
			else {
				const bool stop = lua_isboolean(mLua, -1) && lua_toboolean(mLua, -1) == 0;
				lua_pop(mLua, 1);
				if (stop) break;
			}
			++handler;
		}
	}

	void Host::deliverNativeEvents() {
		for (const auto& eventData : mPendingNativeEvents) {
			++mNativeEventsDelivered;
			if (eventData.type == NativeEventType::UiModeChanged) {
				pushBridgeEventTable(mLua, eventData.previous, eventData.name);
				callEventHandlersWithValue("UiModeChanged", "PLAYER", -1);
				lua_pop(mLua, 1);
			}
			else if (eventData.type == NativeEventType::PlayerDied) {
				lua_pushnil(mLua);
				callEventHandlersWithValue("Died", "PLAYER", -1);
				lua_pop(mLua, 1);
			}
			else if (eventData.type == NativeEventType::SkillLevelUp) callSkillLevelUpHandlers(eventData);
		}
		mPendingNativeEvents.clear();
	}

	int Host::inputRegisterActionThunk(lua_State* state) {
		auto* host = static_cast<Host*>(lua_touserdata(state, lua_upvalueindex(1)));
		if (host == nullptr) return luaL_error(state, "invalid openmw.input package handle");
		luaL_checktype(state, 1, LUA_TTABLE);
		lua_getfield(state, 1, "key"); size_t keySize = 0; const char* keyValue = luaL_checklstring(state, -1, &keySize); std::string key(keyValue, keySize); lua_pop(state, 1);
		if (key.empty() || key.size() > MaxActionKeyLength) return luaL_error(state, "action key must contain 1 to 256 bytes");
		lua_getfield(state, 1, "type"); const auto type = static_cast<ActionType>(luaL_checkinteger(state, -1)); lua_pop(state, 1);
		if (type != ActionType::Boolean && type != ActionType::Number && type != ActionType::Range) return luaL_error(state, "unknown action type");
		lua_getfield(state, 1, "l10n"); size_t l10nSize = 0; luaL_checklstring(state, -1, &l10nSize); lua_pop(state, 1);
		if (l10nSize == 0) return luaL_error(state, "action localization context cannot be empty");
		lua_getfield(state, 1, "defaultValue");
		double defaultValue = 0.0;
		if (type == ActionType::Boolean) { if (!lua_isboolean(state, -1)) return luaL_error(state, "Boolean action defaultValue must be a boolean"); defaultValue = lua_toboolean(state, -1) ? 1.0 : 0.0; }
		else { defaultValue = luaL_checknumber(state, -1); if (!std::isfinite(defaultValue) || (type == ActionType::Range && (defaultValue < 0.0 || defaultValue > 1.0))) return luaL_error(state, "invalid numeric action defaultValue"); }
		lua_pop(state, 1);
		if (host->mInputActions.contains(key)) return luaL_error(state, "Action key \"%s\" is already in use", key.c_str());
		host->mInputActions.emplace(std::move(key), InputAction{ type, defaultValue, defaultValue, {} });
		++host->mActionsRegistered;
		return 0;
	}

	int Host::inputRegisterActionHandlerThunk(lua_State* state) {
		auto* host = static_cast<Host*>(lua_touserdata(state, lua_upvalueindex(1)));
		const auto instanceId = static_cast<std::uint32_t>(lua_tointeger(state, lua_upvalueindex(2)));
		if (host == nullptr || host->findInstance(instanceId) == nullptr) return luaL_error(state, "invalid openmw.input package handle");
		size_t keySize = 0; const char* keyValue = luaL_checklstring(state, 1, &keySize); std::string key(keyValue, keySize);
		auto action = host->mInputActions.find(key);
		if (action == host->mInputActions.end()) return luaL_error(state, "Unknown action key: \"%s\"", key.c_str());
		if (!isCallable(state, 2)) return luaL_error(state, "action handler must be callable");
		lua_pushvalue(state, 2);
		action->second.handlers.push_back({ instanceId, luaL_ref(state, LUA_REGISTRYINDEX) });
		return 0;
	}

	int Host::inputGetActionValueThunk(lua_State* state) {
		auto* host = static_cast<Host*>(lua_touserdata(state, lua_upvalueindex(1)));
		const auto expected = static_cast<ActionType>(lua_tointeger(state, lua_upvalueindex(2)));
		size_t keySize = 0; const char* keyValue = luaL_checklstring(state, 1, &keySize);
		if (host == nullptr) return luaL_error(state, "invalid openmw.input package handle");
		auto action = host->mInputActions.find(std::string(keyValue, keySize));
		if (action == host->mInputActions.end()) return luaL_error(state, "Unknown action key");
		if (action->second.type != expected) return luaL_error(state, "action value type mismatch");
		if (expected == ActionType::Boolean) lua_pushboolean(state, action->second.value != 0.0); else lua_pushnumber(state, action->second.value);
		return 1;
	}

	void Host::pushInputPackage(ScriptInstance& instance) {
		lua_newtable(mLua);
		lua_newtable(mLua);
		lua_pushinteger(mLua, static_cast<lua_Integer>(ActionType::Boolean)); lua_setfield(mLua, -2, "Boolean");
		lua_pushinteger(mLua, static_cast<lua_Integer>(ActionType::Number)); lua_setfield(mLua, -2, "Number");
		lua_pushinteger(mLua, static_cast<lua_Integer>(ActionType::Range)); lua_setfield(mLua, -2, "Range");
		pushReadOnlyProxy(-1, true); lua_remove(mLua, -2); lua_setfield(mLua, -2, "ACTION_TYPE");
		lua_pushlightuserdata(mLua, this); lua_pushcclosure(mLua, &inputRegisterActionThunk, 1); lua_setfield(mLua, -2, "registerAction");
		lua_pushlightuserdata(mLua, this); lua_pushinteger(mLua, instance.id); lua_pushcclosure(mLua, &inputRegisterActionHandlerThunk, 2); lua_setfield(mLua, -2, "registerActionHandler");
		auto getter = [&](const char* name, ActionType type) { lua_pushlightuserdata(mLua, this); lua_pushinteger(mLua, static_cast<lua_Integer>(type)); lua_pushcclosure(mLua, &inputGetActionValueThunk, 2); lua_setfield(mLua, -2, name); };
		getter("getBooleanActionValue", ActionType::Boolean); getter("getNumberActionValue", ActionType::Number); getter("getRangeActionValue", ActionType::Range);
		pushReadOnlyProxy(-1, false); lua_remove(mLua, -2);
	}

	int Host::objectSendEventThunk(lua_State* state) {
		auto* host = static_cast<Host*>(lua_touserdata(state, lua_upvalueindex(1)));
		auto* object = static_cast<ObjectUserdata*>(luaL_checkudata(state, 1, ObjectMetatable));
		if (host == nullptr || object == nullptr || object->host != host) return luaL_error(state, "sendEvent expects a GameObject from this runtime");
		const Status validation = host->mCallbacks.validateHandle(host->mCallbacks.gameUserData, object->handle, HandleType::Player);
		if (validation != Status::Ok) { ++host->mRejectedHandles; return luaL_error(state, "sendEvent rejected player handle with bridge status %u", static_cast<unsigned>(validation)); }
		size_t nameSize = 0; const char* name = luaL_checklstring(state, 2, &nameSize);
		if (nameSize == 0 || nameSize > MaxActionKeyLength) return luaL_error(state, "event name must contain 1 to 256 bytes");
		std::set<const void*> visited; cloneEventValue(state, 3, visited); const int payload = luaL_ref(state, LUA_REGISTRYINDEX);
		DelayedEvent event{}; event.sequence = host->mNextEventSequence++; event.name.assign(name, nameSize); event.targetContainer = "PLAYER"; event.payloadReference = payload;
		host->mNextEvents.push_back(std::move(event)); ++host->mLocalEventsQueued;
		return 0;
	}

	int Host::skillProgressionAddLevelUpHandlerThunk(lua_State* state) {
		auto* host = static_cast<Host*>(lua_touserdata(state, lua_upvalueindex(1)));
		const auto instanceId = static_cast<std::uint32_t>(lua_tointeger(state, lua_upvalueindex(2)));
		if (host == nullptr || host->findInstance(instanceId) == nullptr || !isCallable(state, 1)) return luaL_error(state, "skill level-up handler must be callable");
		lua_pushvalue(state, 1); host->mSkillLevelUpHandlers.push_back({ instanceId, luaL_ref(state, LUA_REGISTRYINDEX) }); return 0;
	}

	void Host::pushSkillProgressionInterface(std::uint32_t instanceId) {
		lua_newtable(mLua);
		lua_pushinteger(mLua, 2); lua_setfield(mLua, -2, "version");
		lua_pushlightuserdata(mLua, this); lua_pushinteger(mLua, instanceId); lua_pushcclosure(mLua, &skillProgressionAddLevelUpHandlerThunk, 2); lua_setfield(mLua, -2, "addSkillLevelUpHandler");
		lua_newtable(mLua); lua_pushliteral(mLua, "book"); lua_setfield(mLua, -2, "Book"); lua_pushliteral(mLua, "usage"); lua_setfield(mLua, -2, "Usage"); lua_pushliteral(mLua, "trainer"); lua_setfield(mLua, -2, "Trainer"); lua_pushliteral(mLua, "jail"); lua_setfield(mLua, -2, "Jail"); pushReadOnlyProxy(-1, true); lua_remove(mLua, -2); lua_setfield(mLua, -2, "SKILL_INCREASE_SOURCES");
		pushReadOnlyProxy(-1, false); lua_remove(mLua, -2);
	}

	int Host::compatibilityActionUpdateThunk(lua_State* state) {
		auto* host = static_cast<Host*>(lua_touserdata(state, lua_upvalueindex(1)));
		if (host == nullptr || (host->mConfig.flags & InitializationHarnessMode) == 0) return luaL_error(state, "action injection is available only in harness mode");
		size_t keySize = 0; const char* key = luaL_checklstring(state, 1, &keySize); const bool value = lua_toboolean(state, 2) != 0;
		ActionUpdate updateData{ sizeof(ActionUpdate), BridgeAbiVersion, ActionType::Boolean, 0, { key, static_cast<std::uint32_t>(keySize) }, value ? 1.0 : 0.0 };
		const Status status = host->updateAction(&updateData); if (status != Status::Ok) return luaL_error(state, "action injection failed with bridge status %u", static_cast<unsigned>(status)); return 0;
	}

	int Host::compatibilityNativeEventThunk(lua_State* state) {
		auto* host = static_cast<Host*>(lua_touserdata(state, lua_upvalueindex(1)));
		if (host == nullptr || (host->mConfig.flags & InitializationHarnessMode) == 0) return luaL_error(state, "native event injection is available only in harness mode");
		const auto type = static_cast<NativeEventType>(luaL_checkinteger(state, 1)); NativeEvent eventData{}; eventData.structureSize = sizeof(eventData); eventData.abiVersion = BridgeAbiVersion; eventData.type = type;
		if (type == NativeEventType::SkillLevelUp) { eventData.index = static_cast<std::uint32_t>(luaL_checkinteger(state, 2)); eventData.value = luaL_checknumber(state, 3); size_t size = 0; const char* source = luaL_optlstring(state, 4, "progress", &size); eventData.source.size = static_cast<std::uint32_t>(std::min(size, static_cast<size_t>(BridgeTextCapacity))); std::memcpy(eventData.source.data, source, eventData.source.size); }
		else if (type == NativeEventType::UiModeChanged) { size_t oldSize = 0, newSize = 0; const char* oldMode = luaL_checklstring(state, 2, &oldSize); const char* newMode = luaL_checklstring(state, 3, &newSize); if (oldSize > BridgeTextCapacity || newSize > BridgeTextCapacity) return luaL_error(state, "UI mode exceeds bridge bounds"); eventData.previous.size = static_cast<std::uint32_t>(oldSize); std::memcpy(eventData.previous.data, oldMode, oldSize); eventData.name.size = static_cast<std::uint32_t>(newSize); std::memcpy(eventData.name.data, newMode, newSize); }
		const Status status = host->queueNativeEvent(&eventData); if (status != Status::Ok) return luaL_error(state, "native event injection failed with bridge status %u", static_cast<unsigned>(status)); return 0;
	}
}
