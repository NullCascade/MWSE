#pragma once

#include <cstdint>

namespace NI {
	struct AVObject;
	struct Camera;
	struct Node;
	struct Object;
	struct ObjectNET;
	struct Pick;
	struct PickRecord;
	struct RTTI;
	struct SwitchNode;
	struct Transform;
	struct TriShape;

	struct Object_vTable;
	struct AVObject_vTable;
	struct Node_vTable;

	namespace RTTIStaticPtr {
		enum RTTIStaticPtr : uintptr_t {
			Accumulator = 0x7DECC0,
			AlphaAccumulator = 0x7DDF48,
			AVObject = 0x7DE7CC,
			Camera = 0x7DDEF8,
			ClusterAccumulator = 0x7DDF20,
			ExtraData = 0x7DDE68,
			Geometry = 0x7DEA2C,
			Object = 0x7DE734,
			ObjectNET = 0x7DE76C,
			MirroredNode = 0x7C8AF8,
			Node = 0x7DDEB8,
			Property = 0x7DE720,
			SwitchNode = 0x7DE1AC,
			TES3ExtraData = 0x7CE280
		};
	}
}
