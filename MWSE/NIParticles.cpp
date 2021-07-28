#include "NIParticles.h"

namespace NI {
	nonstd::span<float> ParticlesData::getSizes() {
		if (sizes) {
			return nonstd::span(sizes, vertexCount);
		}
		return {};
	}

	nonstd::span<TES3::Vector4> RotatingParticlesData::getRotations() {
		if (rotations) {
			return nonstd::span(rotations, vertexCount);
		}
		return {};
	}
}