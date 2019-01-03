#pragma once

#include "TES3Collections.h"
#include "TES3UIVector.h"

#include "LuaUtil.h"

#include <cstddef>

namespace mwse {
	namespace lua {
		//
		// TES3::Iterator<T>
		//

		template <typename T>
		struct Iterator_state {
			TES3::IteratorNode<T> * it;

			Iterator_state(TES3::Iterator<T> * list) {
				it = list->head;
			}
		};

		std::tuple<sol::object, sol::object> bindGenericObjectIterator_pairsNext(sol::user<Iterator_state<TES3::BaseObject>&> user_it_state, sol::this_state l);

		template <typename T>
		std::tuple<sol::object, sol::object> bindIterator_pairsNext(sol::user<Iterator_state<T>&> user_it_state, sol::this_state l) {
			Iterator_state<T>& it_state = user_it_state;
			if (it_state.it == nullptr) {
				return std::make_tuple(sol::object(sol::lua_nil), sol::object(sol::lua_nil));
			}

			auto values = std::make_tuple(sol::object(l, sol::in_place, it_state.it), sol::object(l, sol::in_place, it_state.it->data));
			it_state.it = it_state.it->next;
			return values;
		}

		template <typename T>
		void bindGenericObjectIterator(const char* name, const char* nodeName = NULL) {
			sol::state& state = LuaManager::getInstance().getState();

			if (nodeName) {
				// Start our usertype. We must finish this with state.set_usertype.
				auto usertypeDefinition = state.create_simple_usertype<TES3::IteratorNode<T>>();
				usertypeDefinition.set("new", sol::no_constructor);

				//
				usertypeDefinition.set("previousNode", sol::readonly_property(&TES3::IteratorNode<T>::previous));
				usertypeDefinition.set("nextNode", sol::readonly_property(&TES3::IteratorNode<T>::next));
				usertypeDefinition.set("nodeData", sol::readonly_property([](TES3::IteratorNode<T>& self) { return makeLuaObject(self.data); }));

				// Finish up our usertype.
				state.set_usertype(nodeName, usertypeDefinition);
			}

			// Start our usertype. We must finish this with state.set_usertype.
			auto usertypeDefinition = state.create_simple_usertype<TES3::Iterator<T>>();
			usertypeDefinition.set("new", sol::no_constructor);

			// Metamethod binding.
			usertypeDefinition.set(sol::meta_function::pairs, [](TES3::Iterator<T> * self) {
				Iterator_state<TES3::BaseObject> it_state(reinterpret_cast<TES3::Iterator<TES3::BaseObject>*>(self));
				return std::make_tuple(&bindGenericObjectIterator_pairsNext, sol::user<Iterator_state<TES3::BaseObject>>(std::move(it_state)), sol::lua_nil);
			});
			usertypeDefinition.set(sol::meta_function::index, [](TES3::Iterator<T>& self, int index) {
				TES3::IteratorNode<T>* node = self.head;
				for (int i = 1; i < index; i++) {
					node = node->next;
				}
				return makeLuaObject(node->data);
			});
			usertypeDefinition.set(sol::meta_function::length, [](TES3::Iterator<T>& self) { return self.size; });

			//
			usertypeDefinition.set("size", sol::readonly_property(&TES3::Iterator<T>::size));
			usertypeDefinition.set("head", sol::readonly_property(&TES3::Iterator<T>::head));
			usertypeDefinition.set("tail", sol::readonly_property(&TES3::Iterator<T>::tail));
			usertypeDefinition.set("current", sol::readonly_property(&TES3::Iterator<T>::current));

			// Finish up our usertype.
			state.set_usertype(name, usertypeDefinition);
		}

		template <typename T>
		void bindIterator(const char* name, const char* nodeName = NULL) {
			sol::state& state = LuaManager::getInstance().getState();

			if (nodeName) {
				// Start our usertype. We must finish this with state.set_usertype.
				auto usertypeDefinition = state.create_simple_usertype<TES3::IteratorNode<T>>();
				usertypeDefinition.set("new", sol::no_constructor);

				//
				usertypeDefinition.set("previousNode", sol::readonly_property(&TES3::IteratorNode<T>::previous));
				usertypeDefinition.set("nextNode", sol::readonly_property(&TES3::IteratorNode<T>::next));
				usertypeDefinition.set("nodeData", sol::readonly_property([](TES3::IteratorNode<T>& self) { return self.data; }));

				// Finish up our usertype.
				state.set_usertype(nodeName, usertypeDefinition);
			}

			// Start our usertype. We must finish this with state.set_usertype.
			auto usertypeDefinition = state.create_simple_usertype<TES3::Iterator<T>>();
			usertypeDefinition.set("new", sol::no_constructor);

			// Metamethod binding.
			usertypeDefinition.set(sol::meta_function::pairs, [](TES3::Iterator<T> * self) {
				Iterator_state<T> it_state(self);
				return std::make_tuple(&bindIterator_pairsNext<T>, sol::user<Iterator_state<T>>(std::move(it_state)), sol::lua_nil);
			});
			usertypeDefinition.set(sol::meta_function::index, [](TES3::Iterator<T>& self, int index) {
				TES3::IteratorNode<T>* node = self.head;
				for (int i = 1; i < index; i++) {
					node = node->next;
				}
				return node->data;
			});
			usertypeDefinition.set(sol::meta_function::length, [](TES3::Iterator<T>& self) { return self.size; });

			//
			usertypeDefinition.set("size", sol::readonly_property(&TES3::Iterator<T>::size));
			usertypeDefinition.set("head", sol::readonly_property(&TES3::Iterator<T>::head));
			usertypeDefinition.set("tail", sol::readonly_property(&TES3::Iterator<T>::tail));
			usertypeDefinition.set("current", sol::readonly_property(&TES3::Iterator<T>::current));

			// Finish up our usertype.
			state.set_usertype(name, usertypeDefinition);
		}

		//
		// TES3::TArray<T>
		//

		template <typename T>
		struct TArray_iteratorState {
			TES3::TArray<T> * tarray;
			int index;

			TArray_iteratorState(TES3::TArray<T> * ta) {
				tarray = ta;
				index = 0;
			}
		};

		template <typename T>
		sol::optional<std::tuple<int, sol::object>> bindTArray_pairsIter(sol::user<TArray_iteratorState<T>&> user_state, sol::this_state l) {
			TArray_iteratorState<T>& state = user_state;
			if (state.index >= state.tarray->filledCount) {
				return sol::optional<std::tuple<int, sol::object>>();
			}

			auto values = std::make_tuple(state.index, sol::object(l, sol::in_place, state.tarray->storage[state.index]));
			state.index++;
			return values;
		}

		template <typename T>
		sol::optional<std::tuple<int, sol::object>> bindTArray_ipairsIter(TES3::TArray<T> * tarray, int index, sol::this_state l) {
			if (++index > tarray->filledCount) {
				return sol::optional<std::tuple<int, sol::object>>();
			}

			return std::make_tuple(index, sol::object(l, sol::in_place, tarray->storage[index - 1]));
		}

		template <typename T>
		void bindTArray(const char* name) {
			sol::state& state = LuaManager::getInstance().getState();

			// Start our usertype. We must finish this with state.set_usertype.
			auto usertypeDefinition = state.create_simple_usertype<TES3::TArray<T>>();
			usertypeDefinition.set("new", sol::no_constructor);

			// Metafunction access.
			usertypeDefinition.set(sol::meta_function::pairs, [](TES3::TArray<T> * self) {
				TArray_iteratorState<T> it_state(self);
				return std::make_tuple(&bindTArray_pairsIter<T>, sol::user<TArray_iteratorState<T>>(std::move(it_state)), sol::lua_nil);
			});
			usertypeDefinition.set(sol::meta_function::ipairs, [](TES3::TArray<T> * self) {
				return std::make_tuple(&bindTArray_ipairsIter<T>, self, 0);
			});
			usertypeDefinition.set(sol::meta_function::index, [](TES3::TArray<T>& self, int index) {
				index--;
				if (index < 0 || index > self.endIndex) {
					throw std::out_of_range("Access index out of bounds.");
				}
				return self.storage[index];
			});
			usertypeDefinition.set(sol::meta_function::length, [](TES3::TArray<T>& self) { return self.endIndex; });

			// Function binding.
			usertypeDefinition.set("getIndexOfValue", [](TES3::TArray<T> * self, T * value) {
				return self->getIndexOfValue(value);
			});
			usertypeDefinition.set("contains", [](TES3::TArray<T> * self, T * value) {
				return self->contains(value);
			});

			// Finish up our usertype.
			state.set_usertype(name, usertypeDefinition);
		}

		//
		// TES3::StlList<T>
		//

		struct GenericObjectStlList_iteratorState {
			TES3::StlListNode<TES3::Object> * it;

			GenericObjectStlList_iteratorState(TES3::StlList<TES3::Object> * list) {
				it = list->head;
			}
		};

		std::tuple<sol::object, sol::object> bindGenericObjectStlList_pairsNext(sol::user<GenericObjectStlList_iteratorState&> user_it_state, sol::this_state l);

		template <typename T>
		void bindGenericObjectStlList(const char* name, const char* nodeName = NULL) {
			sol::state& state = LuaManager::getInstance().getState();

			if (nodeName) {
				// Start our usertype. We must finish this with state.set_usertype.
				auto usertypeDefinition = state.create_simple_usertype<TES3::StlListNode<T>>();
				usertypeDefinition.set("new", sol::no_constructor);

				//
				usertypeDefinition.set("previousNode", sol::readonly_property(&TES3::StlListNode<T>::previous));
				usertypeDefinition.set("nextNode", sol::readonly_property(&TES3::StlListNode<T>::next));
				usertypeDefinition.set("nodeData", sol::readonly_property([](TES3::StlListNode<T>& self) { return makeLuaObject(self.data); }));

				// Finish up our usertype.
				state.set_usertype(nodeName, usertypeDefinition);
			}

			// Start our usertype. We must finish this with state.set_usertype.
			auto usertypeDefinition = state.create_simple_usertype<TES3::StlList<T>>();
			usertypeDefinition.set("new", sol::no_constructor);

			// Metamethod binding.
			usertypeDefinition.set(sol::meta_function::pairs, [](TES3::StlList<T> * self) {
				GenericObjectStlList_iteratorState it_state(reinterpret_cast<TES3::StlList<TES3::Object>*>(self));
				return std::make_tuple(&bindGenericObjectStlList_pairsNext, sol::user<GenericObjectStlList_iteratorState>(std::move(it_state)), sol::lua_nil);
			});
			usertypeDefinition.set(sol::meta_function::index, [](TES3::StlList<T>& self, int index) {
				TES3::StlListNode<T>* node = self.head;
				for (int i = 1; i < index; i++) {
					node = node->next;
				}
				return makeLuaObject(node->data);
			});
			usertypeDefinition.set(sol::meta_function::length, [](TES3::StlList<T>& self, int index) {
				return self.size;
			});

			// Basic property binding.
			usertypeDefinition.set("size", sol::readonly_property(&TES3::StlList<T>::size));
			usertypeDefinition.set("head", sol::readonly_property(&TES3::StlList<T>::head));
			usertypeDefinition.set("tail", sol::readonly_property(&TES3::StlList<T>::tail));

			// Finish up our usertype.
			state.set_usertype(name, usertypeDefinition);
		}

		//
		//
		//

		void bindTES3Collections();
	}
}
