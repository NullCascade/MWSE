#pragma once

#include "NINode.h"

#include "BitUtil.h"

namespace NI {
	struct CollisionSwitch : Node {
		static constexpr unsigned short flagCollision = 0x20;

		CollisionSwitch();

		bool getCollisionActive();
		void setCollisionActive(bool active);

		Pointer<CollisionSwitch> create();
	};
	static_assert(sizeof(CollisionSwitch) == 0xB0, "NI::CollisionSwitch failed size validation");
}