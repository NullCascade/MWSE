#pragma once

namespace mwse {
	// Container for registers, flags, and other information to help with the
	// native to MWSE code bridge.
	struct HookContext {
		// General registers.
		DWORD eax;
		DWORD ebx;
		DWORD ecx;
		DWORD edx;

		// String operators.
		DWORD esi;
		DWORD edi;

		// Stack.
		DWORD ebp;
		DWORD esp;

		// Flags (cmp, test).
		DWORD flags;

		// Address to return to.
		DWORD callbackAddress;
	};

	// Code to generate a jump in memory. Don't forget to unprotect it first!
	void genJump(DWORD Address, DWORD To);

	// Code to generate a call in memory. Don't forget to unprotect it first!
	void genCall(DWORD Address, DWORD To);

	// Code to generate a nop in memory. Don't forget to unprotect it first!
	void genNOP(DWORD Address);

	// Code to generate a call in memory. This function unprotects the memory.
	void genJumpUnprotected(DWORD address, DWORD to, DWORD size = 0x5);

	// Code to generate a call in memory. This function unprotects the memory.
	bool genJumpEnforced(DWORD address, DWORD previousTo, DWORD to, DWORD size = 0x5);

	// Code to generate a call in memory. This function unprotects the memory.
	void genCallUnprotected(DWORD address, DWORD to, DWORD size = 0x5);

	// Code to generate a call in memory. This function unprotects the memory.
	bool genCallEnforced(DWORD address, DWORD previousTo, DWORD to, DWORD size = 0x5);

	bool genPushEnforced(DWORD address, BYTE value);
	bool genPushEnforced(DWORD address, DWORD value);

	bool genPushUnprotected(DWORD address, DWORD value);

	// Overrides a function in a virtual table.
	void overrideVirtualTable(DWORD address, DWORD offset, DWORD to);

	// Overrides a function in a virtual table.
	bool overrideVirtualTableEnforced(DWORD address, DWORD offset, DWORD previousTo, DWORD to);

	// Write NOP instructions over a space.
	bool genNOPUnprotected(DWORD address, DWORD size);

	// Write a single value to memory.
	void writeByteUnprotected(DWORD address, BYTE value);
	void writeDoubleWordUnprotected(DWORD address, DWORD value);
	bool writeDoubleWordEnforced(DWORD address, DWORD previousValue, DWORD value);

	// Code to write a patch to a code segment. This function unprotects the memory.
	// WARNING: If passing a function address, always use a non-static function or it will crash.
	void writePatchCodeUnprotected(DWORD address, const BYTE* patch, DWORD size);

	// Code to determine what function an address calls.
	DWORD getCallAddress(DWORD address);

	namespace tes3 {
		//
		// Memory (de)allocation functions.
		//
		// Morrowind is compiled as a release build, and so typical new/delete/malloc/realloc/free
		// calls will not pass debug checks. Anything that comes from or is passed to Morrowind that
		// might be later freed needs to use these functions.
		//

		template <typename T>
		inline constexpr T* _new(size_t count = 1) {
			return reinterpret_cast<T * (__cdecl*)(size_t)>(0x727692)(sizeof(T) * count);
		}

		void* _new(size_t size);

		template <typename T>
		inline constexpr void _delete(T* address) {
			const auto __delete = reinterpret_cast<void(__cdecl*)(T*)>(0x727530);
			__delete(address);
		}

		void* realloc(void* address, size_t size);

		void* malloc(size_t size);

		template <typename T>
		inline constexpr T* malloc() {
			T* ret = reinterpret_cast<T*>(malloc(sizeof(T)));
			memset(ret, 0, sizeof(T));
			return ret;
		}

		void free(void* address);
	}
}

