#include "NIObjectNET.h"

namespace NI {
	const auto NI_ObjectNET_setName = reinterpret_cast<void(__thiscall*)(const ObjectNET*, const char *)>(0x6EA1A0);

	void ObjectNET::setName(const char* name) {
		NI_ObjectNET_setName(this, name);
	}
}
