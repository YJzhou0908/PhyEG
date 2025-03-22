#include "GLRenderState.h"

namespace pe {

	GLRenderState::GLRenderState() noexcept {

	}

	GLRenderState::~GLRenderState() noexcept {}

	void GLRenderState::init() noexcept {
		mLightsArray.clear();
		mShadowsArray.clear();
		mLights->init();
	}

	//更新了一些与坐标系选择无关的uniform-color
	void GLRenderState::setupLights() noexcept {
		mLights->setupLights(mLightsArray);
	}

	//更新与坐标系选择有关的uniforms-direction
	void GLRenderState::setupLightsView(const Camera::Ptr& camera) noexcept {
		mLights->setupLightsView(mLightsArray, camera);
	}

	void GLRenderState::pushLight(const Light::Ptr& light) noexcept {
		mLightsArray.push_back(light);
	}

	void GLRenderState::pushShadow(const Light::Ptr& shadowLight) noexcept {
		mShadowsArray.push_back(shadowLight);
	}

}