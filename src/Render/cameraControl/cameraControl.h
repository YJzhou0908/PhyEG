#pragma once 
#include "../basic/common.h"
#include "../basic/constants.h"
#include "../camera/camera.h"
namespace pe {

	class CameraControl {
	public:
		using Ptr = std::shared_ptr<CameraControl>;

		CameraControl(const Camera::Ptr& camera) noexcept;

		~CameraControl() noexcept;

		virtual void onKeyboard(const KeyBoardState& action) noexcept;

		virtual void onMouseAction(const MouseAction& action) noexcept;

		virtual void onMouseMove(double xpos, double ypos) noexcept;

	protected:
		Camera::Ptr	mCamera{ nullptr };//正交相机  透视相机

	};
}