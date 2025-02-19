#include "orthographicCamera.h"

namespace pe {

	OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top, float near, float far) noexcept {
		mLeft = left;
		mRight = right;
		mBottom = bottom;
		mNear = near;
		mFar = far;

		updateProjectionMatrix();
	
	}

	glm::mat4 OrthographicCamera::updateProjectionMatrix() noexcept {
		mProjectionMatrix = glm::ortho(mLeft, mRight, mBottom, mTop, mNear, mFar);
		return mProjectionMatrix;
	}

}
