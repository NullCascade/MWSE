#include "pch.h"
#include "CppUnitTest.h"

#define MWSE_NO_CUSTOM_ALLOC 1
#include "..\MWSE\NITArray.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace CustomContainers {
	TEST_CLASS(NiTArray) {
public:

	//
	// Lua test functions.
	//

	static void lua_log(sol::this_state ts, sol::object object) {
		sol::state_view state = ts;
		std::string result = state["tostring"](object);
		result += "\n";
		Logger::WriteMessage(result.c_str());
	}

	TEST_METHOD(LuaLength) {
		NI::TArray<int> container(10);
		for (size_t i = 0; i < container.size(); i++) {
			container.setAtIndex(i, rand() % 1000);
		}

		sol::state lua;
		lua.open_libraries();

		try {
			lua["container"] = &container;
			size_t length = lua.safe_script("return #container");
			Assert::AreEqual(container.size(), length);
		}
		catch (std::exception& e) {
			std::wstringstream ss;
			ss << "Lua exception: " << e.what();
			Assert::Fail(ss.str().c_str());
		}
	}

	TEST_METHOD(LuaIPairs) {
		NI::TArray<int> container(13);
		container.fill(5);

		sol::state lua;
		lua.open_libraries();

		try {
			lua["container"] = &container;
			size_t sum = lua.safe_script(R"(
				local sum = 0
				for _, value in ipairs(container) do
					sum = sum + value
					print(string.format("Value: %d; Sum: %d", value, sum))
				end
				return sum
				)");
			Assert::AreEqual(container.size() * 5, sum);
		}
		catch (std::exception& e) {
			std::wstringstream ss;
			ss << "Lua exception: " << e.what();
			Assert::Fail(ss.str().c_str());
		}
	}

	TEST_METHOD(LuaPairs) {
		NI::TArray<int> container(6);
		container.fill(5);

		sol::state lua;
		lua.open_libraries();
		lua["print"] = &lua_log;
		lua["container"] = &container;

		try {
			size_t sum = lua.safe_script(R"(
				local sum = 0
				for _, value in pairs(container) do
					sum = sum + value
					print(string.format("Value: %d; Sum: %d", value, sum))
				end
				return sum
				)");
			Assert::AreEqual(container.size() * 5, sum);
		}
		catch (std::exception& e) {
			std::wstringstream ss;
			ss << "Lua exception: " << e.what();
			Assert::Fail(ss.str().c_str());
		}
	}

	};
}
