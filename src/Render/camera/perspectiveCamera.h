#pragma once
#include "../basic/common.h"
#include "camera.h"

namespace pe {
	class PerspectiveCamera : public Camera {
	public:
		using Ptr = std::shared_ptr<PerspectiveCamera>;

		static Ptr create(float near, float far, float aspect, float fov) {
			return std::make_shared <PerspectiveCamera>(near, far, aspect, fov);
		}
		//aspect ��ʾ��߱ȣ� fov��ʾ�ӽǳ�
		PerspectiveCamera(float near, float far, float aspect, float fov) noexcept;

		~PerspectiveCamera() noexcept;
	private:
		glm::mat4 updateProjectionMatrix() noexcept override;
	private:
		float mFar{ 100.0f };
		float mNear{ 0.1f };
		float mAspect{ 1.3f }; // 4:3��߱�
		float mFov{ 45.0f };
	
	
	};


}