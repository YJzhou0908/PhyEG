#pragma once
#include "directionalLight.h"

namespace pe {

	DirectionalLight::DirectionalLight() noexcept {
		mIsDirectionalLight = true;
		mShadow = DirectionalLightShadow::create();
	}

	DirectionalLight::~DirectionalLight() noexcept {}
}