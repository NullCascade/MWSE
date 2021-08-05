#include "NiTriBasedGeometryData.h"

namespace NI {
	unsigned short TriBasedGeometryData::getActiveTriangleCount() const {
		return vTable.asTriBasedGeometryData->getActiveTriangleCount(this);
	}

	void TriBasedGeometryData::setActiveTriangleCount(unsigned short count) {
		return vTable.asTriBasedGeometryData->setActiveTriangleCount(this, count);
	}
}
