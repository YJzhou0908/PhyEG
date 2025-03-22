#include "perspectiveCamera.h"

namespace pe {

	PerspectiveCamera::PerspectiveCamera(float near, float far, float aspect, float fov) noexcept {
		mIsPerspectiveCamera = true;
		mNear = near;
		mFar = far;
		mAspect = aspect;
		mFov = fov;
		updateProjectionMatrix();
	}

	PerspectiveCamera::~PerspectiveCamera() {};

	// ProjectMatrix������ת��ΪNDC����
	glm::mat4 PerspectiveCamera::updateProjectionMatrix() noexcept {
		mProjectionMatrix = glm::perspective(glm::radians(mFov), mAspect, mNear, mFar);

		return mProjectionMatrix;
	}


}