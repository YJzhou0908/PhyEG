#include "ambientLight.h"

namespace pe {

	AmbientLight::AmbientLight() noexcept {
		mIsAmbientLight = true;
		mIntensity = 0.1f;
	}

	AmbientLight::~AmbientLight() noexcept {}
}