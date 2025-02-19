#include "cameraControl.h"

namespace pe {
	CameraControl::CameraControl(const Camera::Ptr& camera) noexcept {
		mCamera = camera;
	}

	CameraControl::~CameraControl() noexcept {}

	void CameraControl::onKeyboard(const KeyBoardState& action) noexcept {}

	void CameraControl::onMouseAction(const MouseAction& action) noexcept {}

	void CameraControl::onMouseMove(double xpos, double ypos) noexcept {}

}