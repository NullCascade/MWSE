#include "OpenMWLuaHost.h"

#include "TES3Birthsign.h"
#include "TES3Cell.h"
#include "TES3Class.h"
#include "TES3DataHandler.h"
#include "TES3GlobalVariable.h"
#include "TES3GameSetting.h"
#include "TES3MagicEffect.h"
#include "TES3MagicEffectController.h"
#include "TES3MagicSourceInstance.h"
#include "TES3MobilePlayer.h"
#include "TES3NPC.h"
#include "TES3Race.h"
#include "TES3Region.h"
#include "TES3Skill.h"
#include "TES3Spell.h"
#include "TES3SpellList.h"
#include "TES3Statistic.h"
#include "TES3Util.h"
#include "TES3WorldController.h"
#include "Log.h"

namespace mwse::openmw {
	namespace {
		constexpr const char* AttributeIds[] = {
			"strength", "intelligence", "willpower", "agility", "speed", "endurance", "personality", "luck"
		};
		constexpr const char* SkillIds[] = {
			"block", "armorer", "mediumarmor", "heavyarmor", "bluntweapon", "longblade", "axe", "spear",
			"athletics", "enchant", "destruction", "alteration", "illusion", "conjuration", "mysticism",
			"restoration", "alchemy", "unarmored", "security", "sneak", "acrobatics", "lightarmor",
			"shortblade", "marksman", "mercantile", "speechcraft", "handtohand"
		};
		constexpr const char* EffectIds[] = {
			"waterbreathing", "swiftswim", "waterwalking", "shield", "fireshield", "lightningshield", "frostshield", "burden", "feather", "jump", "levitate", "slowfall", "lock", "open", "firedamage", "shockdamage", "frostdamage", "drainattribute", "drainhealth", "drainmagicka", "drainfatigue", "drainskill", "damageattribute", "damagehealth", "damagemagicka", "damagefatigue", "damageskill", "poison", "weaknesstofire", "weaknesstofrost", "weaknesstoshock", "weaknesstomagicka", "weaknesstocommondisease", "weaknesstoblightdisease", "weaknesstocorprusdisease", "weaknesstopoison", "weaknesstonormalweapons", "disintegrateweapon", "disintegratearmor", "invisibility", "chameleon", "light", "sanctuary", "nighteye", "charm", "paralyze", "silence", "blind", "sound", "calmhumanoid", "calmcreature", "frenzyhumanoid", "frenzycreature", "demoralizehumanoid", "demoralizecreature", "rallyhumanoid", "rallycreature", "dispel", "soultrap", "telekinesis", "mark", "recall", "divineintervention", "almsiviintervention", "detectanimal", "detectenchantment", "detectkey", "spellabsorption", "reflect", "curecommondisease", "cureblightdisease", "curecorprusdisease", "curepoison", "cureparalyzation", "restoreattribute", "restorehealth", "restoremagicka", "restorefatigue", "restoreskill", "fortifyattribute", "fortifyhealth", "fortifymagicka", "fortifyfatigue", "fortifyskill", "fortifymaximummagicka", "absorbattribute", "absorbhealth", "absorbmagicka", "absorbfatigue", "absorbskill", "resistfire", "resistfrost", "resistshock", "resistmagicka", "resistcommondisease", "resistblightdisease", "resistcorprusdisease", "resistpoison", "resistnormalweapons", "resistparalysis", "removecurse", "turnundead", "summonscamp", "summonclannfear", "summondaedroth", "summondremora", "summonancestralghost", "summonskeletalminion", "summonbonewalker", "summongreaterbonewalker", "summonbonelord", "summonwingedtwilight", "summonhunger", "summongoldensaint", "summonflameatronach", "summonfrostatronach", "summonstormatronach", "fortifyattack", "commandcreature", "commandhumanoid", "bounddagger", "boundlongsword", "boundmace", "boundbattleaxe", "boundspear", "boundlongbow", "extraspell", "boundcuirass", "boundhelm", "boundboots", "boundshield", "boundgloves", "corprus", "vampirism", "summoncenturionsphere", "sundamage", "stuntedmagicka", "summonfabricant", "summonwolf", "summonbear", "summonbonewolf", "summoncreature04", "summoncreature05"
		};

		bool setText(BridgeText& destination, const char* source, bool lower = false) {
			destination = {};
			if (source == nullptr) return true;
			const std::size_t size = strnlen(source, BridgeTextCapacity + 1);
			if (size > BridgeTextCapacity) return false;
			destination.size = static_cast<std::uint32_t>(size);
			for (std::size_t i = 0; i < size; ++i) destination.data[i] = lower
				? static_cast<char>(std::tolower(static_cast<unsigned char>(source[i]))) : source[i];
			return true;
		}

		bool validSnapshot(const void* value, std::uint32_t structureSize, std::uint32_t abiVersion,
			std::uint32_t expectedSize) {
			return value != nullptr && structureSize == expectedSize && abiVersion == BridgeAbiVersion;
		}

		std::string queryId(StringView id) {
			if (id.size > BridgeTextCapacity || (id.size != 0 && id.data == nullptr)) return {};
			std::string result(id.data, id.size);
			std::transform(result.begin(), result.end(), result.begin(), [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
			return result;
		}

		TES3::MobilePlayer* currentPlayer() {
			auto* world = TES3::WorldController::get();
			return world ? world->getMobilePlayer() : nullptr;
		}

		const char* effectId(int id) {
			return id >= 0 && id < static_cast<int>(std::size(EffectIds)) ? EffectIds[id] : nullptr;
		}

		bool fillEffect(EffectSnapshot& output, const TES3::Effect& effect, double magnitude) {
			output = {};
			const char* id = effectId(effect.effectID);
			if (id != nullptr) {
				if (!setText(output.id, id)) return false;
			}
			else {
				const std::string nativeId = "mwse:effect:" + std::to_string(effect.effectID);
				if (!setText(output.id, nativeId.c_str())) return false;
			}
			if (effect.skillID >= TES3::SkillID::FirstSkill && effect.skillID <= TES3::SkillID::LastSkill)
				setText(output.affectedSkill, SkillIds[effect.skillID]);
			if (effect.attributeID >= TES3::Attribute::FirstAttribute && effect.attributeID <= TES3::Attribute::LastAttribute)
				setText(output.affectedAttribute, AttributeIds[effect.attributeID]);
			output.magnitudeMin = effect.magnitudeMin;
			output.magnitudeMax = effect.magnitudeMax;
			output.magnitudeThisFrame = magnitude;
			output.duration = effect.duration;
			output.area = effect.radius;
			output.range = static_cast<std::uint32_t>(effect.rangeType);
			auto* data = TES3::DataHandler::get();
			auto* magic = data && data->nonDynamicData ? data->nonDynamicData->magicEffects->getEffectObject(effect.effectID) : nullptr;
			if (magic) {
				output.baseCost = magic->baseMagickaCost;
				output.school = magic->school;
				output.flags = magic->getEffectFlags();
			}
			return true;
		}

		Status fillSpellRecord(TES3::Spell* spell, RecordSnapshot* snapshot, std::uint32_t index) {
			if (!spell) return Status::NotFound;
			RecordSnapshot result{};
			result.structureSize = sizeof(result); result.abiVersion = BridgeAbiVersion;
			result.type = RecordType::Spell; result.index = index;
			if (!setText(result.id, spell->getObjectID(), true) || !setText(result.name, spell->name)) {
				log::getLog() << "[OpenMW Lua] Spell record text exceeds bridge bounds: id="
					<< (spell->getObjectID() ? spell->getObjectID() : "<null>") << std::endl;
				return Status::OutOfRange;
			}
			result.specialization = spell->castType;
			result.value = spell->magickaCost;
			if (spell->getAutoCalc()) result.flags |= 1u;
			if (spell->getAlwaysSucceeds()) result.flags |= 2u;
			if (spell->getPlayerStart()) result.flags |= 4u;
			for (const auto& effect : spell->effects) {
				if (effect.effectID == TES3::EffectID::None) continue;
				if (result.itemCount[0] >= MaxRecordEffects) {
					log::getLog() << "[OpenMW Lua] Spell record exceeds bridge effect bound: id="
						<< spell->getObjectID() << std::endl;
					return Status::OutOfRange;
				}
				if (!fillEffect(result.effects[result.itemCount[0]++], effect, 0.0)) {
					log::getLog() << "[OpenMW Lua] Spell record has unsupported effect id: id="
						<< spell->getObjectID() << " effect=" << effect.effectID << std::endl;
					return Status::OutOfRange;
				}
			}
			*snapshot = result;
			return Status::Ok;
		}

		template <typename T>
		T* listRecord(NI::IteratedList<T*>* list, std::uint32_t index, const std::string& id) {
			if (!list) return nullptr;
			std::uint32_t current = 0;
			for (auto* record : *list) {
				if (!record) continue;
				if (!id.empty()) {
					const char* recordId = record->getObjectID();
					if (recordId && _stricmp(recordId, id.c_str()) == 0) return record;
				}
				else if (current++ == index) return record;
			}
			return nullptr;
		}
	}

	void HostController::resetHandles() {
		restoreHarnessMutations();
		++handleGeneration;
		if (handleGeneration == 0) ++handleGeneration;
		handledPlayer = currentPlayer();
		handledCells.clear();
	}

	void HostController::restoreHarnessMutations() {
		if (harnessMutations.empty() || handledPlayer == nullptr || restoringHarnessMutations) return;
		restoringHarnessMutations = true;
		const auto handle = playerOpaqueHandle();
		for (auto it = harnessMutations.rbegin(); it != harnessMutations.rend(); ++it)
			setStat(this, handle, it->kind, it->index, it->field, it->originalValue);
		harnessMutations.clear();
		restoringHarnessMutations = false;
	}

	OpaqueHandle HostController::playerOpaqueHandle() const {
		return { 1, handleGeneration, static_cast<std::uint32_t>(HandleType::Player) };
	}

	OpaqueHandle HostController::cellOpaqueHandle(void* cell) {
		for (std::size_t i = 0; i < handledCells.size(); ++i)
			if (handledCells[i] == cell) return { i + 2, handleGeneration, static_cast<std::uint32_t>(HandleType::Cell) };
		handledCells.push_back(cell);
		return { handledCells.size() + 1, handleGeneration, static_cast<std::uint32_t>(HandleType::Cell) };
	}

	Status HostController::validateOpaqueHandle(OpaqueHandle handle, HandleType expectedType, void** nativeValue) const {
		if (nativeValue) *nativeValue = nullptr;
		if (handle.value == 0 || handle.reserved == 0) return Status::InvalidHandle;
		if (handle.reserved != static_cast<std::uint32_t>(expectedType)) return Status::WrongHandleType;
		if (handle.generation != handleGeneration) return Status::StaleHandle;
		if (expectedType == HandleType::Player) {
			if (handle.value != 1 || handledPlayer == nullptr || currentPlayer() != handledPlayer) return Status::StaleHandle;
			if (nativeValue) *nativeValue = handledPlayer;
			return Status::Ok;
		}
		if (expectedType == HandleType::Cell) {
			if (handle.value < 2 || handle.value - 2 >= handledCells.size()) return Status::InvalidHandle;
			void* cell = handledCells[static_cast<std::size_t>(handle.value - 2)];
			if (!cell) return Status::StaleHandle;
			if (nativeValue) *nativeValue = cell;
			return Status::Ok;
		}
		return Status::WrongHandleType;
	}

	Status __cdecl HostController::validateHandle(void* userData, OpaqueHandle handle, HandleType expectedType) {
		auto* self = static_cast<HostController*>(userData);
		return self ? self->validateOpaqueHandle(handle, expectedType, nullptr) : Status::InvalidArgument;
	}

	Status __cdecl HostController::getPlayerObject(void* userData, ObjectSnapshot* snapshot) {
		auto* self = static_cast<HostController*>(userData);
		if (!self || !snapshot || snapshot->structureSize != sizeof(ObjectSnapshot) || snapshot->abiVersion != BridgeAbiVersion)
			return Status::InvalidArgument;
		void* native = nullptr;
		const auto status = self->validateOpaqueHandle(self->playerOpaqueHandle(), HandleType::Player, &native);
		if (status != Status::Ok) return status;
		auto* player = static_cast<TES3::MobilePlayer*>(native);
		if (!player->npcInstance || !player->npcInstance->baseNPC) return Status::InvalidState;
		ObjectSnapshot result{}; result.structureSize = sizeof(result); result.abiVersion = BridgeAbiVersion;
		result.object = self->playerOpaqueHandle();
		result.typeMask = static_cast<std::uint32_t>(ObjectType::Actor) | static_cast<std::uint32_t>(ObjectType::Npc)
			| static_cast<std::uint32_t>(ObjectType::Player);
		if (!setText(result.recordId, player->npcInstance->baseNPC->getObjectID(), true)) return Status::OutOfRange;
		auto* cell = player->getCell();
		if (cell) result.cell = self->cellOpaqueHandle(cell);
		*snapshot = result;
		return Status::Ok;
	}

	Status __cdecl HostController::getCell(void* userData, OpaqueHandle handle, CellSnapshot* snapshot) {
		auto* self = static_cast<HostController*>(userData);
		if (!self || !snapshot || snapshot->structureSize != sizeof(CellSnapshot) || snapshot->abiVersion != BridgeAbiVersion)
			return Status::InvalidArgument;
		void* native = nullptr; const auto status = self->validateOpaqueHandle(handle, HandleType::Cell, &native);
		if (status != Status::Ok) return status;
		auto* cell = static_cast<TES3::Cell*>(native);
		CellSnapshot result{}; result.structureSize = sizeof(result); result.abiVersion = BridgeAbiVersion; result.handle = handle;
		const bool interior = cell->getIsInterior();
		const bool quasi = cell->getBehavesAsExterior();
		if (!interior) result.flags |= CellIsExterior | CellHasSky;
		if (quasi) result.flags |= CellIsQuasiExterior | CellHasSky;
		if (cell->getHasWater()) result.flags |= CellHasWater;
		if (auto water = cell->getWaterLevel()) { result.flags |= CellHasWaterLevel; result.waterLevel = *water; }
		if (!interior) { result.gridX = cell->getGridX(); result.gridY = cell->getGridY(); setText(result.worldSpaceId, "sys::default"); }
		if (!setText(result.name, cell->getName()) || !setText(result.displayName, cell->getDisplayName())) return Status::OutOfRange;
		if (auto* region = cell->getRegion()) { if (!setText(result.region, region->id, true)) return Status::OutOfRange; }
		if (interior) { if (!setText(result.id, cell->getName(), true)) return Status::OutOfRange; }
		else {
			const std::string id = "#" + std::to_string(result.gridX) + " " + std::to_string(result.gridY);
			if (!setText(result.id, id.c_str(), true)) return Status::OutOfRange;
		}
		*snapshot = result; return Status::Ok;
	}

	Status __cdecl HostController::getStat(void* userData, OpaqueHandle actor, StatKind kind,
		std::uint32_t index, StatSnapshot* snapshot) {
		auto* self = static_cast<HostController*>(userData);
		if (!self || !snapshot || snapshot->structureSize != sizeof(StatSnapshot) || snapshot->abiVersion != BridgeAbiVersion)
			return Status::InvalidArgument;
		void* native = nullptr; const auto status = self->validateOpaqueHandle(actor, HandleType::Player, &native);
		if (status != Status::Ok) return status;
		auto* player = static_cast<TES3::MobilePlayer*>(native);
		StatSnapshot result{}; result.structureSize = sizeof(result); result.abiVersion = BridgeAbiVersion;
		TES3::Statistic* stat = nullptr;
		if (kind == StatKind::Attribute) { if (index >= 8) return Status::OutOfRange; stat = &player->attributes[index]; result.writableMask = (1u << 0) | (1u << 3) | (1u << 4); }
		else if (kind == StatKind::Skill) { if (index >= 27) return Status::OutOfRange; stat = &player->skills[index]; result.writableMask = (1u << 0) | (1u << 3) | (1u << 4) | (1u << 5); }
		else if (kind == StatKind::Health) { stat = &player->health; result.writableMask = (1u << 0) | (1u << 1) | (1u << 3); }
		else if (kind == StatKind::Level) {
			result.current = result.base = result.modified = player->getOpenMWLevel();
			result.progress = player->getOpenMWLevelProgress(); result.writableMask = (1u << 1) | (1u << 5); *snapshot = result; return Status::Ok;
		}
		else return Status::Unsupported;
		result.base = stat->getBase(); result.current = stat->getCurrentRaw(); result.modified = stat->getOpenMWModified();
		result.modifier = stat->getOpenMWModifier(); result.damage = stat->getOpenMWDamage();
		if (kind == StatKind::Skill) result.progress = player->getOpenMWSkillProgress(index);
		*snapshot = result; return Status::Ok;
	}

	Status __cdecl HostController::setStat(void* userData, OpaqueHandle actor, StatKind kind,
		std::uint32_t index, StatField field, double value) {
		auto* self = static_cast<HostController*>(userData);
		if (!self || !std::isfinite(value)) return Status::InvalidArgument;
		void* native = nullptr; const auto status = self->validateOpaqueHandle(actor, HandleType::Player, &native);
		if (status != Status::Ok) return status;
		auto* player = static_cast<TES3::MobilePlayer*>(native);
		if ((self->initializationFlags & InitializationHarnessMode) != 0 && !self->restoringHarnessMutations) {
			const bool seen = std::any_of(self->harnessMutations.begin(), self->harnessMutations.end(),
				[kind,index,field](const HarnessMutation& entry) { return entry.kind==kind&&entry.index==index&&entry.field==field; });
			if (!seen) {
				StatSnapshot original{}; original.structureSize=sizeof(original); original.abiVersion=BridgeAbiVersion;
				if (getStat(self,actor,kind,index,&original)==Status::Ok) {
					double originalValue=0.0;
					switch(field){case StatField::Base:originalValue=original.base;break;case StatField::Current:originalValue=original.current;break;case StatField::Modifier:originalValue=original.modifier;break;case StatField::Damage:originalValue=original.damage;break;case StatField::Progress:originalValue=original.progress;break;default:break;}
					self->harnessMutations.push_back({kind,index,field,originalValue});
				}
			}
		}
		if (kind == StatKind::Level) {
			if (field == StatField::Current) { if (value < 1 || value > 32767) return Status::OutOfRange; player->setOpenMWLevel(static_cast<int>(value)); return Status::Ok; }
			if (field == StatField::Progress) { if (value < 0 || value > INT_MAX) return Status::OutOfRange; player->setOpenMWLevelProgress(static_cast<int>(value)); return Status::Ok; }
			return Status::ReadOnly;
		}
		TES3::Statistic* stat = nullptr;
		if (kind == StatKind::Attribute) { if (index >= 8) return Status::OutOfRange; stat = &player->attributes[index]; }
		else if (kind == StatKind::Skill) { if (index >= 27) return Status::OutOfRange; stat = &player->skills[index]; }
		else if (kind == StatKind::Health) stat = &player->health;
		else return Status::Unsupported;
		if (field == StatField::Base) stat->setOpenMWBase(static_cast<float>(value));
		else if (field == StatField::Modifier) stat->setOpenMWModifier(static_cast<float>(value));
		else if (field == StatField::Damage && kind != StatKind::Health) stat->setOpenMWDamage(static_cast<float>(value));
		else if (field == StatField::Current && kind == StatKind::Health) stat->setCurrentCapped(static_cast<float>(value), false);
		else if (field == StatField::Progress && kind == StatKind::Skill) player->setOpenMWSkillProgress(index, static_cast<float>(value));
		else return Status::ReadOnly;
		return Status::Ok;
	}

	Status __cdecl HostController::getRecordCount(void*, RecordType type, std::uint32_t* count) {
		if (!count) return Status::InvalidArgument;
		auto* data = TES3::DataHandler::get(); auto* ndd = data ? data->nonDynamicData : nullptr;
		if (!ndd) return Status::InvalidState;
		switch (type) {
		case RecordType::Attribute: *count = 8; return Status::Ok;
		case RecordType::Skill: *count = 27; return Status::Ok;
		case RecordType::Npc: *count = 1; return Status::Ok;
		case RecordType::Class: *count = ndd->classes ? static_cast<std::uint32_t>(ndd->classes->size()) : 0; return Status::Ok;
		case RecordType::Race: *count = ndd->races ? static_cast<std::uint32_t>(ndd->races->size()) : 0; return Status::Ok;
		case RecordType::Birthsign: *count = ndd->birthsigns ? static_cast<std::uint32_t>(ndd->birthsigns->size()) : 0; return Status::Ok;
		case RecordType::Spell: *count = ndd->spellsList ? static_cast<std::uint32_t>(ndd->spellsList->size()) : 0; return Status::Ok;
		case RecordType::MagicEffect: *count = static_cast<std::uint32_t>(std::size(EffectIds)); return Status::Ok;
		default: return Status::Unsupported;
		}
	}

	Status __cdecl HostController::getRecord(void*, RecordType type, std::uint32_t index, StringView id,
		RecordSnapshot* snapshot) {
		if (!snapshot || snapshot->structureSize != sizeof(RecordSnapshot) || snapshot->abiVersion != BridgeAbiVersion
			|| id.size > BridgeTextCapacity || (id.size && !id.data)) return Status::InvalidArgument;
		auto* data = TES3::DataHandler::get(); auto* ndd = data ? data->nonDynamicData : nullptr;
		if (!ndd) return Status::InvalidState;
		const std::string key = queryId(id);
		RecordSnapshot result{}; result.structureSize = sizeof(result); result.abiVersion = BridgeAbiVersion; result.type = type; result.index = index;
		if (type == RecordType::Attribute) {
			if (!key.empty()) { index = 8; for (std::uint32_t i=0;i<8;++i) if (key==AttributeIds[i]) { index=i; break; } }
			if (index >= 8) return Status::NotFound;
			setText(result.id, AttributeIds[index]); auto* setting = ndd->GMSTs[mwse::tes3::getAttributeNameGMST(index)];
			if (!setting || !setText(result.name, setting->value.asString)) return Status::InvalidState; *snapshot=result; return Status::Ok;
		}
		if (type == RecordType::Skill) {
			if (!key.empty()) { index = 27; for (std::uint32_t i=0;i<27;++i) if (key==SkillIds[i]) { index=i; break; } }
			if (index >= 27) return Status::NotFound;
			setText(result.id, SkillIds[index]); auto& skill=ndd->skills[index]; result.specialization=skill.specialization;
			if (!setText(result.name, skill.getName())) return Status::OutOfRange; *snapshot=result; return Status::Ok;
		}
		if (type == RecordType::Npc) {
			auto* player=currentPlayer(); auto* npc=player&&player->npcInstance?player->npcInstance->baseNPC:nullptr;
			if (!npc || (!key.empty() && _stricmp(npc->getObjectID(),key.c_str())!=0) || (key.empty()&&index!=0)) return Status::NotFound;
			auto* race = player->npcInstance->getBaseRace();
			auto* playerClass = player->npcInstance->getBaseClass();
			if (!setText(result.id,npc->getObjectID(),true)||!setText(result.name,npc->name)
				||!setText(result.auxiliaryId1,race?race->getObjectID():nullptr,true)
				||!setText(result.auxiliaryId2,playerClass?playerClass->getObjectID():nullptr,true)) return Status::OutOfRange;
			if (!setText(result.itemIds[0], player->birthsign ? player->birthsign->getObjectID() : nullptr, true)) return Status::OutOfRange;
			if (npc->getIsFemale()) result.flags|=1u;
			auto* world=TES3::WorldController::get(); if(world&&world->gvarCharGenState&&world->gvarCharGenState->value==-1.0f)result.flags|=2u;
			*snapshot=result; return Status::Ok;
		}
		if (type == RecordType::Class) {
			auto* record=listRecord(ndd->classes,index,key); if(!record)return Status::NotFound;
			if(!setText(result.id,record->id,true)||!setText(result.name,record->name))return Status::OutOfRange; result.specialization=record->specialization;
			result.itemCount[0]=2;result.itemCount[1]=5;result.itemCount[2]=5;
			for(int i=0;i<2;++i)setText(result.itemIds[i],AttributeIds[record->attributes[i]]);
			for(int i=0;i<5;++i){setText(result.itemIds[2+i],SkillIds[record->skills[i*2]]);setText(result.itemIds[7+i],SkillIds[record->skills[i*2+1]]);}
			*snapshot=result;return Status::Ok;
		}
		if (type == RecordType::Race) {
			auto* record=listRecord(ndd->races,index,key);if(!record)return Status::NotFound;
			if(!setText(result.id,record->id,true)||!setText(result.name,record->name))return Status::OutOfRange; result.itemCount[0]=8;result.itemCount[1]=7;
			for(int i=0;i<8;++i){setText(result.itemIds[i],AttributeIds[i]);result.itemValues[i*2]=record->baseAttributes[i].male;result.itemValues[i*2+1]=record->baseAttributes[i].female;}
			for(int i=0;i<7;++i){setText(result.itemIds[8+i],SkillIds[record->skillBonuses[i].skill]);result.itemValues[16+i]=record->skillBonuses[i].bonus;}
			if(record->abilities){for(auto* spell:*record->abilities){if(!spell)continue;if(15+result.itemCount[2]>=MaxRecordItems)return Status::OutOfRange;setText(result.itemIds[15+result.itemCount[2]++],spell->getObjectID(),true);}}
			*snapshot=result;return Status::Ok;
		}
		if (type == RecordType::Birthsign) {
			auto* record=listRecord(ndd->birthsigns,index,key);if(!record)return Status::NotFound;
			if(!setText(result.id,record->id,true)||!setText(result.name,record->name))return Status::OutOfRange;
			for(auto* spell:record->spellList){if(!spell)continue;if(result.itemCount[0]>=MaxRecordItems)return Status::OutOfRange;setText(result.itemIds[result.itemCount[0]++],spell->getObjectID(),true);}*snapshot=result;return Status::Ok;
		}
		if (type == RecordType::Spell) {
			TES3::Spell* spell=nullptr;if(!key.empty())spell=ndd->getSpellById(key.c_str());else if(ndd->spellsList&&index<ndd->spellsList->size())spell=*(ndd->spellsList->begin()+index);
			return fillSpellRecord(spell,snapshot,index);
		}
		if (type == RecordType::MagicEffect) {
			if(!key.empty()){index=static_cast<std::uint32_t>(std::size(EffectIds));for(std::uint32_t i=0;i<std::size(EffectIds);++i)if(key==EffectIds[i]){index=i;break;}}
			if(index>=std::size(EffectIds))return Status::NotFound;auto* effect=ndd->magicEffects->getEffectObject(index);if(!effect)return Status::NotFound;
			setText(result.id,EffectIds[index]);if(!setText(result.name,effect->getComplexName(-1,-1).c_str()))return Status::OutOfRange;result.specialization=effect->school;result.itemValues[0]=effect->baseMagickaCost;result.flags=effect->getEffectFlags();*snapshot=result;return Status::Ok;
		}
		return Status::Unsupported;
	}

	Status __cdecl HostController::getActorSpellCount(void* userData, OpaqueHandle actor, std::uint32_t* count) {
		if(!count)return Status::InvalidArgument;auto* self=static_cast<HostController*>(userData);void* native=nullptr;auto status=self?self->validateOpaqueHandle(actor,HandleType::Player,&native):Status::InvalidArgument;if(status!=Status::Ok)return status;
		auto* list=static_cast<TES3::MobilePlayer*>(native)->getSpellList();*count=list?static_cast<std::uint32_t>(list->size()):0;return Status::Ok;
	}

	Status __cdecl HostController::getActorSpell(void* userData, OpaqueHandle actor, std::uint32_t index, RecordSnapshot* snapshot) {
		auto* self=static_cast<HostController*>(userData);void* native=nullptr;auto status=self?self->validateOpaqueHandle(actor,HandleType::Player,&native):Status::InvalidArgument;if(status!=Status::Ok)return status;
		auto* list=static_cast<TES3::MobilePlayer*>(native)->getSpellList();if(!list||index>=list->size())return Status::OutOfRange;return fillSpellRecord(*(list->begin()+index),snapshot,index);
	}

	Status __cdecl HostController::setActorSpell(void* userData, OpaqueHandle actor, StringView id, std::uint32_t add) {
		auto* self=static_cast<HostController*>(userData);void* native=nullptr;auto status=self?self->validateOpaqueHandle(actor,HandleType::Player,&native):Status::InvalidArgument;if(status!=Status::Ok)return status;
		const std::string key=queryId(id);if(key.empty())return Status::InvalidArgument;auto* list=static_cast<TES3::MobilePlayer*>(native)->getSpellList();if(!list)return Status::InvalidState;
		if(!TES3::DataHandler::get()->nonDynamicData->getSpellById(key.c_str()))return Status::NotFound;return (add?list->add(key):list->remove(key))?Status::Ok:Status::InvalidState;
	}

	Status __cdecl HostController::getActiveSpellCount(void* userData, OpaqueHandle actor, std::uint32_t* count) {
		if(!count)return Status::InvalidArgument;auto* self=static_cast<HostController*>(userData);void* native=nullptr;auto status=self?self->validateOpaqueHandle(actor,HandleType::Player,&native):Status::InvalidArgument;if(status!=Status::Ok)return status;
		std::set<unsigned int> serials;for(const auto& effect:static_cast<TES3::MobilePlayer*>(native)->activeMagicEffects)serials.insert(effect.magicInstanceSerial);*count=static_cast<std::uint32_t>(serials.size());return Status::Ok;
	}

	Status __cdecl HostController::getActiveSpell(void* userData, OpaqueHandle actor, std::uint32_t index, ActiveSpellSnapshot* snapshot) {
		if(!snapshot||snapshot->structureSize!=sizeof(ActiveSpellSnapshot)||snapshot->abiVersion!=BridgeAbiVersion)return Status::InvalidArgument;auto* self=static_cast<HostController*>(userData);void* native=nullptr;auto status=self?self->validateOpaqueHandle(actor,HandleType::Player,&native):Status::InvalidArgument;if(status!=Status::Ok)return status;
		auto* player=static_cast<TES3::MobilePlayer*>(native);std::vector<unsigned int> serials;for(const auto& effect:player->activeMagicEffects)if(std::find(serials.begin(),serials.end(),effect.magicInstanceSerial)==serials.end())serials.push_back(effect.magicInstanceSerial);if(index>=serials.size())return Status::OutOfRange;
		ActiveSpellSnapshot result{};result.structureSize=sizeof(result);result.abiVersion=BridgeAbiVersion;const auto serial=serials[index];
		for(const auto& active:player->activeMagicEffects){if(active.magicInstanceSerial!=serial)continue;auto* instance=active.getInstance();if(!instance)continue;if(result.id.size==0&&!setText(result.id,instance->magicID,true))return Status::OutOfRange;if(instance->getSourceType()==TES3::MagicSourceType::Spell){auto* spell=instance->sourceCombo.source.asSpell;if(spell&&spell->castType==TES3::SpellCastType::Ability)result.affectsBaseValues=1;}
			if(result.effectCount>=MaxRecordEffects)return Status::OutOfRange;TES3::Effect source{};source.effectID=active.magicEffectID;source.skillID=-1;source.attributeID=-1;auto span=instance->getSourceEffects();if(active.magicInstanceEffectIndex<span.size())source=span[active.magicInstanceEffectIndex];if(active.magicEffectID==TES3::EffectID::FortifySkill)source.skillID=active.skillOrAttributeID;if(active.magicEffectID==TES3::EffectID::FortifyAttribute)source.attributeID=active.skillOrAttributeID;if(!fillEffect(result.effects[result.effectCount++],source,active.getMagnitude()))return Status::OutOfRange;}
		*snapshot=result;return Status::Ok;
	}
}
