#pragma once

#include "sol.hpp"

#include "TES3Defines.h"

namespace mwse {
	namespace lua {
		namespace event {

			sol::object trigger(const char* eventType, sol::table eventData = sol::nil, sol::object eventFilter = sol::nil);

			void clearObjectFilter(sol::object filterObject);

			// ---------------------------------------------------------------------------- //

			class BaseEvent {
			public:
				virtual const char* getEventName() { return NULL; };
				virtual sol::table createEventTable() { return sol::nil; };
				virtual sol::object getEventOptions() { return sol::nil; }
			};

			// ---------------------------------------------------------------------------- //

			class GenericEvent : public BaseEvent {
			public:
				GenericEvent(const char* name);
				const char* getEventName();

			protected:
				const char* m_EventName;
			};

			// ---------------------------------------------------------------------------- //

			class ObjectFilteredEvent : public GenericEvent {
			public:
				ObjectFilteredEvent(const char* name, TES3::BaseObject* filter);
				sol::object getEventOptions();

			protected:
				TES3::BaseObject* m_EventFilter;
			};

			// ---------------------------------------------------------------------------- //

			class EquipEvent : public ObjectFilteredEvent {
			public:
				EquipEvent(TES3::Reference* reference, TES3::BaseObject* item, TES3::ItemData* itemData);
				sol::table createEventTable();

			protected:
				TES3::Reference* m_Reference;
				TES3::BaseObject* m_Item;
				TES3::ItemData* m_ItemData;
			};

			// ---------------------------------------------------------------------------- //

			class EquippedEvent : public ObjectFilteredEvent {
			public:
				EquippedEvent(TES3::Actor* a, TES3::MobileActor* ma, TES3::BaseObject* i, TES3::ItemData* id);
				sol::table createEventTable();

			protected:
				TES3::Actor* m_Actor;
				TES3::MobileActor* m_MobileActor;
				TES3::BaseObject* m_Item;
				TES3::ItemData* m_ItemData;
			};

			// ---------------------------------------------------------------------------- //

			class UnequippedEvent : public EquippedEvent {
			public:
				UnequippedEvent(TES3::Actor* a, TES3::MobileActor* ma, TES3::BaseObject* i, TES3::ItemData* id);
			};

			// ---------------------------------------------------------------------------- //

			class FrameEvent : public GenericEvent {
			public:
				FrameEvent(float delta, bool menuMode);
				sol::table createEventTable();

			protected:
				bool m_MenuMode;
				float m_Delta;
			};

			// ---------------------------------------------------------------------------- //

			class SimulateEvent : public FrameEvent {
			public:
				SimulateEvent(float delta, double timestamp);
				sol::table createEventTable();

			protected:
				double m_Timestamp;
			};

			// ---------------------------------------------------------------------------- //

			class ButtonPressedEvent : public GenericEvent {
			public:
				ButtonPressedEvent(int buttonId);
				sol::table createEventTable();

			protected:
				int m_ButtonId;
			};

			// ---------------------------------------------------------------------------- //

			class ActivateEvent : public ObjectFilteredEvent {
			public:
				ActivateEvent(TES3::Reference* activator, TES3::Reference* target);
				sol::table createEventTable();

			protected:
				TES3::Reference* m_Activator;
				TES3::Reference* m_Target;
			};

			// ---------------------------------------------------------------------------- //

			class SaveGameEvent : public GenericEvent {
			public:
				SaveGameEvent(const char* saveName, const char* fileName);
				sol::table createEventTable();
				sol::object getEventOptions();

			protected:
				const char* m_SaveName;
				const char* m_FileName;
			};

			// ---------------------------------------------------------------------------- //

			class SavedGameEvent : public SaveGameEvent {
			public:
				SavedGameEvent(const char* saveName, const char* fileName);
			};

			// ---------------------------------------------------------------------------- //

			class LoadGameEvent : public GenericEvent {
			public:
				LoadGameEvent(const char* fileName, bool quickLoad = false, bool newGame = false);
				sol::table createEventTable();
				sol::object getEventOptions();

			protected:
				const char* m_FileName;
				bool m_QuickLoad;
				bool m_NewGame;
			};

			// ---------------------------------------------------------------------------- //

			class LoadedGameEvent : public LoadGameEvent {
			public:
				LoadedGameEvent(const char* fileName, bool quickLoad = false, bool newGame = false);
			};

			// ---------------------------------------------------------------------------- //

			class CellChangedEvent : public ObjectFilteredEvent {
			public:
				CellChangedEvent(TES3::Cell* cell, TES3::Cell* previousCell);
				sol::table createEventTable();

			protected:
				TES3::Cell* m_Cell;
				TES3::Cell* m_PreviousCell;
			};

			// ---------------------------------------------------------------------------- //

			class CombatStartEvent : public ObjectFilteredEvent {
			public:
				CombatStartEvent(TES3::MobileActor* mobileActor, TES3::MobileActor* target);
				sol::table createEventTable();

			protected:
				TES3::MobileActor* m_MobileActor;
				TES3::MobileActor* m_Target;
			};

			// ---------------------------------------------------------------------------- //

			class CombatStartedEvent : public ObjectFilteredEvent {
			public:
				CombatStartedEvent(TES3::MobileActor* mobileActor, TES3::MobileActor* target);
				sol::table createEventTable();

			protected:
				TES3::MobileActor* m_MobileActor;
				TES3::MobileActor* m_Target;
			};

			// ---------------------------------------------------------------------------- //

			class CombatStopEvent : public ObjectFilteredEvent {
			public:
				CombatStopEvent(TES3::MobileActor* mobileActor);
				sol::table createEventTable();

			protected:
				TES3::MobileActor* m_MobileActor;
			};

			// ---------------------------------------------------------------------------- //

			class CombatStoppedEvent : public ObjectFilteredEvent {
			public:
				CombatStoppedEvent(TES3::MobileActor* mobileActor);
				sol::table createEventTable();

			protected:
				TES3::MobileActor* m_MobileActor;
			};

			// ---------------------------------------------------------------------------- //

			class AttackEvent : public ObjectFilteredEvent {
			public:
				AttackEvent(TES3::ActorAnimationData* animData);
				sol::table createEventTable();

			protected:
				TES3::ActorAnimationData* m_AnimationData;
			};

			// ---------------------------------------------------------------------------- //

			class MobileObjectCollisionEvent : public ObjectFilteredEvent {
			public:
				MobileObjectCollisionEvent(TES3::MobileObject* mobileObject, TES3::Reference* targetReference);
				sol::table createEventTable();

			protected:
				TES3::MobileObject* m_MobileObject;
				TES3::Reference* m_TargetReference;
			};

			// ---------------------------------------------------------------------------- //

			class MobileProjectileActorCollisionEvent : public ObjectFilteredEvent {
			public:
				MobileProjectileActorCollisionEvent(TES3::MobileProjectile* projectile, TES3::Reference* targetReference);
				sol::table createEventTable();

			protected:
				TES3::MobileProjectile* m_Projectile;
				TES3::Reference* m_TargetReference;
			};

			// ---------------------------------------------------------------------------- //

			class ProjectileExpireEvent : public ObjectFilteredEvent {
			public:
				ProjectileExpireEvent(TES3::MobileProjectile* projectile);
				sol::table createEventTable();

			protected:
				TES3::MobileProjectile* m_Projectile;
			};

			// ---------------------------------------------------------------------------- //

			class MobileObjectWaterImpactEvent : public ObjectFilteredEvent {
			public:
				MobileObjectWaterImpactEvent(TES3::MobileObject* mobileObject, bool inWater);
				sol::table createEventTable();

			protected:
				TES3::MobileObject* m_MobileObject;
				bool m_InWater;
			};

			// ---------------------------------------------------------------------------- //

			class GenericUiPreEvent : public GenericEvent {
			public:
				GenericUiPreEvent(TES3::UI::Element* parent, TES3::UI::Element* element, unsigned int prop, unsigned int var1, unsigned int var2);
				sol::table createEventTable();

			protected:
				TES3::UI::Element* m_Parent;
				TES3::UI::Element* m_Source;
				unsigned int m_Property;
				unsigned int m_Variable1;
				unsigned int m_Variable2;
			};

			// ---------------------------------------------------------------------------- //

			class GenericUiPostEvent : public GenericUiPreEvent {
			public:
				GenericUiPostEvent(TES3::UI::Element* parent, TES3::UI::Element* element, unsigned int prop, unsigned int var1, unsigned int var2);
			};

			// ---------------------------------------------------------------------------- //

			class GenericUiActivatedEvent : public GenericEvent {
			public:
				GenericUiActivatedEvent(TES3::UI::Element * parent);
				sol::table createEventTable();
				sol::object getEventOptions();

			protected:
				TES3::UI::Element* m_Element;
				bool m_Created;
			};

			// TODO: Remove deprecated event.
			class GenericUiCreatedEvent : public GenericUiActivatedEvent {
			public:
				GenericUiCreatedEvent(TES3::UI::Element * parent);
			};

			// ---------------------------------------------------------------------------- //

			class ShowRestWaitMenuEvent : public GenericEvent {
			public:
				ShowRestWaitMenuEvent(bool allowRest, bool scripted);
				sol::table createEventTable();

			protected:
				bool m_AllowRest;
				bool m_Scripted;
			};

			// ---------------------------------------------------------------------------- //

			class KeyEvent : public GenericEvent {
			public:
				KeyEvent(int keyCode, bool pressed, bool controlDown, bool shiftDown, bool altDown, bool superDown);
				sol::table createEventTable();
				sol::object getEventOptions();

			protected:
				int m_KeyCode;
				bool m_Pressed;
				bool m_ControlDown;
				bool m_ShiftDown;
				bool m_AltDown;
				bool m_SuperDown;
			};

			// ---------------------------------------------------------------------------- //

			class KeyDownEvent : public KeyEvent {
			public:
				KeyDownEvent(int keyCode, bool controlDown, bool shiftDown, bool altDown, bool superDown);
			};

			// ---------------------------------------------------------------------------- //

			class KeyUpEvent : public KeyEvent {
			public:
				KeyUpEvent(int keyCode, bool controlDown, bool shiftDown, bool altDown, bool superDown);
			};

			// ---------------------------------------------------------------------------- //

			class MouseButtonDownEvent : public KeyEvent {
			public:
				MouseButtonDownEvent(int button, bool controlDown, bool shiftDown, bool altDown, bool superDown);
				sol::table createEventTable();
			};

			// ---------------------------------------------------------------------------- //

			class MouseButtonUpEvent : public KeyEvent {
			public:
				MouseButtonUpEvent(int button, bool controlDown, bool shiftDown, bool altDown, bool superDown);
				sol::table createEventTable();
			};

			// ---------------------------------------------------------------------------- //

			class MouseAxisEvent : public GenericEvent {
			public:
				MouseAxisEvent(int deltaX, int deltaY, bool controlDown, bool shiftDown, bool altDown, bool superDown);
				sol::table createEventTable();

			protected:
				int m_DeltaX;
				int m_DeltaY;
				bool m_ControlDown;
				bool m_ShiftDown;
				bool m_AltDown;
				bool m_SuperDown;
			};

			// ---------------------------------------------------------------------------- //

			class MouseWheelEvent : public GenericEvent {
			public:
				MouseWheelEvent(int delta, bool controlDown, bool shiftDown, bool altDown, bool superDown);
				sol::table createEventTable();

			protected:
				int m_Delta;
				bool m_ControlDown;
				bool m_ShiftDown;
				bool m_AltDown;
				bool m_SuperDown;
			};

			// ---------------------------------------------------------------------------- //

			class DeathEvent : public ObjectFilteredEvent {
			public:
				DeathEvent(TES3::MobileActor* mobileActor);
				sol::table createEventTable();

			protected:
				TES3::MobileActor * m_MobileActor;
			};

			// ---------------------------------------------------------------------------- //

			class DamageEvent : public ObjectFilteredEvent {
			public:
				DamageEvent(TES3::MobileActor* mobileActor, float damage);
				sol::table createEventTable();

			protected:
				TES3::MobileActor * m_MobileActor;
				float m_Damage;
			};

			// ---------------------------------------------------------------------------- //

			class DamagedEvent : public ObjectFilteredEvent {
			public:
				DamagedEvent(TES3::MobileActor* mobileActor, float damage);
				sol::table createEventTable();

			protected:
				TES3::MobileActor * m_MobileActor;
				float m_Damage;
			};

			// ---------------------------------------------------------------------------- //

			class SkillExerciseEvent : public GenericEvent {
			public:
				SkillExerciseEvent(int skillId, float progress);
				sol::table createEventTable();
				sol::object getEventOptions();

			protected:
				int m_Skill;
				float m_Progress;
			};

			// ---------------------------------------------------------------------------- //

			class LevelUpEvent : public GenericEvent {
			public:
				LevelUpEvent();
				sol::table createEventTable();
				sol::object getEventOptions();
			};

			// ---------------------------------------------------------------------------- //

			class SpellTickEvent : public GenericEvent {
			public:
				SpellTickEvent(int effectId, TES3::MagicSourceInstance * sourceInstance, float deltaTime, TES3::MagicEffectInstance * effectInstance, int effectIndex);
				sol::table createEventTable();
				sol::object getEventOptions();

			protected:
				int m_EffectId;
				TES3::MagicSourceInstance * m_SourceInstance;
				float m_DeltaTime;
				TES3::MagicEffectInstance * m_EffectInstance;
				int m_EffectIndex;
			};

			// ---------------------------------------------------------------------------- //

			class SpellResistEvent : public GenericEvent {
			public:
				SpellResistEvent(TES3::MagicSourceInstance * spellInstance, TES3::MagicEffectInstance * effectInstance, int effectIndex, int resistAttribute);
				sol::table createEventTable();
				sol::object getEventOptions();

			protected:
				TES3::MagicSourceInstance* m_MagicSourceInstance;
				TES3::MagicEffectInstance* m_EffectInstance;
				int m_EffectIndex;
				int m_ResistAttribute;
			};

			// ---------------------------------------------------------------------------- //

			class PotionBrewedEvent : public GenericEvent {
			public:
				PotionBrewedEvent(TES3::Alchemy * object);
				sol::table createEventTable();

			protected:
				TES3::Alchemy * m_Object;
			};

			// ---------------------------------------------------------------------------- //

			class MenuStateEvent : public BaseEvent {
			public:
				MenuStateEvent(bool inMenuMode);
				const char* getEventName();
				sol::table createEventTable();

			protected:
				bool m_InMenuMode;
			};

			// ---------------------------------------------------------------------------- //

			class MagicCastedEvent : public GenericEvent {
			public:
				MagicCastedEvent(TES3::MagicSourceInstance* magicInstance);
				sol::table createEventTable();
				sol::object getEventOptions();

			protected:
				TES3::MagicSourceInstance* m_MagicSourceInstance;
			};

			class SpellCastEvent : public GenericEvent {
			public:
				SpellCastEvent(TES3::Spell* spell, TES3::MobileActor* caster, float castChance, int weakestSchool);
				sol::table createEventTable();
				sol::object getEventOptions();

			protected:
				TES3::Spell* m_Spell;
				TES3::MobileActor* m_Caster;
				float m_CastChance;
				int m_WeakestSchool;
			};

			class SpellCastedEvent : public GenericEvent {
			public:
				SpellCastedEvent(TES3::MagicSourceInstance* magicInstance, bool success, int expGainSchool);
				sol::table createEventTable();
				sol::object getEventOptions();

			protected:
				TES3::MagicSourceInstance* m_MagicSourceInstance;
				int m_ExpGainSchool;
			};

			// ---------------------------------------------------------------------------- //

			class CalculateMovementSpeed : public ObjectFilteredEvent {
			public:
				enum MovementType {
					Move,
					Walk,
					Run,
					Swim,
					SwimRun,
					Fly
				};

				CalculateMovementSpeed(MovementType type, TES3::MobileActor * mobile, float speed);
				sol::table createEventTable();

			protected:
				MovementType m_Type;
				TES3::MobileActor * m_MobileActor;
				float m_Speed;
			};

			// ---------------------------------------------------------------------------- //

			class CalcRestInterruptEvent : public GenericEvent {
			public:
				CalcRestInterruptEvent(int count, int hour);
				sol::table createEventTable();

			protected:
				int m_Count;
				int m_Hour;
			};

			class RestInterruptEvent : public GenericEvent {
			public:
				RestInterruptEvent(TES3::LeveledCreature * leveledCreature);
				sol::table createEventTable();

			protected:
				TES3::LeveledCreature * m_Creature;
			};

			// ---------------------------------------------------------------------------- //

			class AddTopicEvent : public GenericEvent {
			public:
				AddTopicEvent(TES3::Dialogue * topic);
				sol::table createEventTable();

			protected:
				TES3::Dialogue * m_Topic;
			};

			// ---------------------------------------------------------------------------- //

			class JournalEvent : public GenericEvent {
			public:
				JournalEvent(TES3::Dialogue * topic, int oldIndex, int newIndex);
				sol::table createEventTable();

			protected:
				TES3::Dialogue * m_Topic;
				int m_OldIndex;
				int m_NewIndex;
			};

			// ---------------------------------------------------------------------------- //

			class ActivationTargetChangedEvent : public ObjectFilteredEvent {
			public:
				ActivationTargetChangedEvent(TES3::Reference* previous, TES3::Reference* current);
				sol::table createEventTable();

			protected:
				TES3::Reference* m_PreviousReference;
				TES3::Reference* m_CurrentReference;
			};

			// ---------------------------------------------------------------------------- //

			class WeatherCycledEvent : public GenericEvent {
			public:
				WeatherCycledEvent();
			};

			class WeatherChangedImmediateEvent : public GenericEvent {
			public:
				WeatherChangedImmediateEvent();
				sol::table createEventTable();
			};

			class WeatherTransitionStartedEvent : public GenericEvent {
			public:
				WeatherTransitionStartedEvent();
				sol::table createEventTable();
			};

			class WeatherTransitionFinishedEvent : public GenericEvent {
			public:
				WeatherTransitionFinishedEvent();
				sol::table createEventTable();
			};

			// ---------------------------------------------------------------------------- //

			class MusicSelectTrackEvent : public GenericEvent {
			public:
				MusicSelectTrackEvent(int situation);
				sol::table createEventTable();

			protected:
				int m_Situation;
			};

		}
	}
}
