#pragma once
#include "../../basic/common.h"
#include "../../basic/constants.h"
#include "GLLights.h"
#include "../../light/light.h"
#include "../../light/lightShadow.h"
#include "../../light/directionalLight.h"
#include "../../camera/camera.h"

namespace pe {

	//存储了光与影
	class GLRenderState {
	public:
		using Ptr = std::shared_ptr<GLRenderState>;
		static Ptr create() {
			return std::make_shared<GLRenderState>();
		}

		GLRenderState() noexcept;

		~GLRenderState() noexcept;

		//每一帧，都会调用init，清理掉所有的数组
		void init() noexcept;

		void setupLights() noexcept;

		void setupLightsView(const Camera::Ptr& camera) noexcept;

		void pushLight(const Light::Ptr& light) noexcept;

		void pushShadow(const Light::Ptr& shadowLight) noexcept;

	public:
		GLLights::Ptr mLights = GLLights::create();

		std::vector<Light::Ptr> mLightsArray{};//所有场景当中的光源
		std::vector<Light::Ptr> mShadowsArray{};//所有场景当中可以产生阴影的光源
	};
}