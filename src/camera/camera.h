#pragma once
#include "../basic/common.h"
#include "../core/object3D.h"
namespace pe {
	class Camera : public Object3D {
	public:
		using Ptr = std::shared_ptr<Camera>;

		Camera() noexcept;

		~Camera() noexcept;

		glm::mat4 updateWorldMatrix(bool updateParent = false, bool updateChildren = false) noexcept override;


		glm::mat4 getWorldMatrixInverse() noexcept { return mWorldMatrixInverse; }

		glm::mat4 getProjectionMatrix() noexcept { return mProjectionMatrix; }

		virtual glm::mat4 updateProjectionMatrix() noexcept = 0;//纯虚函数,需要在子类当中实现

	protected:
		// WorldMatrixInverse就是ViewMatrix。
		glm::mat4 mWorldMatrixInverse = glm::mat4(1.0f);
		glm::mat4 mProjectionMatrix = glm::mat4(1.0f);
	};


}