#include "NISwitchNode.h"

namespace NI {
	int SwitchNode::getSwitchIndex() {
		return switchIndex;
	}

	void SwitchNode::setSwitchIndex(int index) {
		// Index -1 indicates no child active.
		if (index < -1 || index > int(children.getFilledCount() - 1) || children.at(index) == nullptr) {
			throw std::exception("Attempted to set switchIndex beyond bounds!");
		}
		switchIndex = index;
	}

	Pointer<AVObject> SwitchNode::getActiveChild() const {
		if (switchIndex >= 0) {
			return children.at(switchIndex);
		}
		return nullptr;
	}
}

MWSE_SOL_CUSTOMIZED_PUSHER_DEFINE_NI(NI::SwitchNode)
