#pragma once 
#include "../basic/common.h"
#include "../core/object3D.h"

namespace pe {
	class OrthographicCamera : public Object3D {
	public:
		using Ptr = std::shared_ptr<OrthographicCamera>;


		OrthographicCamera(float left, float right, float bottom, float top, float near, float far) noexcept;

		~OrthographicCamera() noexcept;

	private:
		glm::mat4 updateProjectionMatrix() noexcept override;

	private:
		// 六个平面描述正交投影矩阵
		float mLeft{ 0.0f };
		float mRight{ 0.0f };
		float mTop{ 0.0f };
		float mBottom{ 0.0f };
		float mNear{ 0.0f };
		float mFar{ 0.0f };


	};


}