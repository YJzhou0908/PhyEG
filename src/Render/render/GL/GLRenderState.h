#pragma once
#include "../../basic/common.h"
#include "../../basic/constants.h"
#include "GLLights.h"
#include "../../light/light.h"
#include "../../light/lightShadow.h"
#include "../../light/directionalLight.h"
#include "../../camera/camera.h"

namespace pe {

	//�洢�˹���Ӱ
	class GLRenderState {
	public:
		using Ptr = std::shared_ptr<GLRenderState>;
		static Ptr create() {
			return std::make_shared<GLRenderState>();
		}

		GLRenderState() noexcept;

		~GLRenderState() noexcept;

		//ÿһ֡���������init����������е�����
		void init() noexcept;

		void setupLights() noexcept;

		void setupLightsView(const Camera::Ptr& camera) noexcept;

		void pushLight(const Light::Ptr& light) noexcept;

		void pushShadow(const Light::Ptr& shadowLight) noexcept;

	public:
		GLLights::Ptr mLights = GLLights::create();

		std::vector<Light::Ptr> mLightsArray{};//���г������еĹ�Դ
		std::vector<Light::Ptr> mShadowsArray{};//���г������п��Բ�����Ӱ�Ĺ�Դ
	};
}