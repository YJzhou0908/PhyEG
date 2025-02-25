#pragma once
#include "../../basic/common.h"
#include "../../basic/constants.h"

namespace pe {
	class Renderer;

	class GLWindow {
	public:
		using FrameSizeCallback = std::function<void(GLWindow*, int width, int height);

		using MouseMoveCallback = std::function<void(double xpos, double ypos)>;

		using MouseActionCallback = std::function<void(MouseAction)>;

		using KeyboardActionCallback = std::function<void(KeyBoardState)>;

		using Ptr = std::shared_ptr<GLWindow>;
		static Ptr create(Renderer* renderer, const int& width, const int& height) {
			return std::make_shared<GLWindow>(renderer, width, height);
		}


		GLWindow(Renderer* renderer, int width, int height ) noexcept;
		~GLWindow() noexcept;

		//设置各类事件的回调函数
		void setFrameSizeCallBack(const FrameSizeCallback& callback) noexcept;

		void setMouseMoveCallBack(const MouseMoveCallback& callback) noexcept;

		void setMouseActionCallback(const MouseActionCallback& callback) noexcept;

		void setKeyboardActionCallBack(const KeyboardActionCallback& callback) noexcept;


		//每一帧渲染前，都会调用processEvent，对事件进行响应
		bool processEvent() noexcept;

		//双缓冲的更换
		void swap() noexcept;

		GLFWwindow* getWindow() const  noexcept { return mWindow; }

	private:
		// 私有化glfw相关事件，会先回调下方函数，如果外界已经给到window注册的接口，那么在下方函数中会调用外界回调函数
		static void frameSizeCallback(GLFWwindow* window, int width, int height) noexcept;

		static void mouseMoveCallback(GLFWwindow* window, double xpos, double ypos) noexcept;

		static void mouseActionCallback(GLFWwindow* window, int button, int action, int mods) noexcept;

	public:
		Renderer* mRender{ nullptr };

	private:
		int mWidth{ 0 };
		int mHeight{ 0 };

		KeyBoardState mKeyState{};

		//允许外界设置相关回调函数
		FrameSizeCallback		mFrameSizeCallback{ nullptr };
		MouseMoveCallback		mMouseMoveCallback{ nullptr };
		KeyboardActionCallback	mKeyboardActionCallback{ nullptr };
		MouseActionCallback		mMouseActionCallback{ nullptr };

		GLFWwindow* mWindow{ NULL };
	
	};

}
