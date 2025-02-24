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
		//aspect 表示宽高比， fov表示视角场
		PerspectiveCamera(float near, float far, float aspect, float fov) noexcept;

		~PerspectiveCamera() noexcept;
	private:
		glm::mat4 updateProjectionMatrix() noexcept override;
	private:
		float mFar{ 100.0f };
		float mNear{ 0.1f };
		float mAspect{ 1.3f }; // 4:3宽高比
		float mFov{ 45.0f };
	
	
	};


}