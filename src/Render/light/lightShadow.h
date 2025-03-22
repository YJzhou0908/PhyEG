#pragma once
#include "../basic/common.h"
#include "../camera/camera.h"
#include "../math/frustum.h"
#include "../render/renderTarget.h"

namespace pe {

	class Light;

	class LightShadow {
	public:
		using Ptr = std::shared_ptr<LightShadow>;
		static Ptr create(const Camera::Ptr& camera) {
			return std::make_shared<LightShadow>(camera);
		}

		LightShadow(const Camera::Ptr& camera) noexcept;

		~LightShadow() noexcept;

		virtual void updateMatrices(const std::shared_ptr<Light>& light) noexcept;

		uint32_t getViewportCount() const noexcept;

		glm::vec4 getViewport(const uint32_t& index);

		Frustum::Ptr getFrustum() const noexcept;

	public:
		Camera::Ptr				mCamera{ nullptr };//���ڹ�Դλ�ã�������Ⱦ�����ͼ�������
		float					mBias{ -0.003f };//Ϊ�˷�ֹShadowAnce�����ƫ����
		float					mRadius{ 1.0f };//��ֹ��Ե��Ӳ��ʱ�򣬲��õ������Ͳ�����Χ�Ĵ�С

		glm::vec2				mMapSize = glm::vec2(512.0, 512.0);//ShadowMap�ֱ��ʵĴ�С 


		glm::vec2				mFrameExtent = glm::vec2(1.0, 1.0);
		std::vector<glm::vec4>	mViewports = { glm::vec4(0.0f, 0.0f, 1.0f, 1.0f) };

		RenderTarget::Ptr		mRenderTarget{ nullptr };//��ǰ��ShadowMap����Ӧ����ȾĿ��,ShadowMap�ͷ���������ColorAttachment

		glm::mat4				mMatrix = glm::mat4(1.0f);

	protected:
		//����������Դ����������Ӿ������
		Frustum::Ptr			mFrustum = Frustum::create();
	};
}