#include "GLWindow.h"
#include "../render.h"

namespace pe {

	GLWindow::GLWindow(Renderer* renderer, int width, int height) noexcept {
		mRender = renderer;
		mWidth = width;
		mHeight = height;

		glfwInit();

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);

		// 创建窗体
		mWindow = glfwCreateWindow(mWidth, mHeight, "Window", nullptr, nullptr);

		if (!mWindow) {
			std::cerr << "Error: failed to create window" << std::endl;
			glfwTerminate();
			exit(0);
		}

		glfwMakeContextCurrent(mWindow);

		// 加载OpenGL的API
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			std::cout << "Failed to initialize GLAD" << std::endl;
			exit(0);
		}

		//将当前的GLWindow这个类的对象的this指针，作为了一个userData，传给了GLFWWindow（mWindow）
		glfwSetWindowUserPointer(mWindow, this);

		//注册回调函数
		glfwSetFramebufferSizeCallback(mWindow, frameSizeCallback);

		glfwSetMouseButtonCallback(mWindow, mouseActionCallback);

		glfwSetCursorPosCallback(mWindow, mouseMoveCallback);

		//close the v-sync for screen 
		// 垂直同步的关闭，显示器有一定的刷新频率，大概率比渲染速度要慢很多，开启
		// 垂直同步的时候，渲染速度会与刷新频率一致
		//glfwSwapInterval(0);
	}

	GLWindow::~GLWindow() noexcept  {
		glfwDestroyWindow(mWindow);
		glfwTerminate();
	}

	void GLWindow::swap() noexcept {
		glfwSwapBuffers(mWindow);
	}


	void GLWindow::setFrameSizeCallBack(const FrameSizeCallback& callback) noexcept {
		mFrameSizeCallback = callback;
	}

	void GLWindow::setMouseMoveCallBack(const MouseMoveCallback& callback) noexcept {
		mMouseMoveCallback = callback;
	}

	void GLWindow::setMouseActionCallback(const MouseActionCallback& callback) noexcept {
		mMouseActionCallback = callback;
	}

	void GLWindow::setKeyboardActionCallBack(const KeyboardActionCallback& callback) noexcept {
		mKeyboardActionCallback = callback;
	}


	// 调用注册的回调函数
	void GLWindow::frameSizeCallback(GLFWwindow* window, int width, int height) noexcept {
		auto self = static_cast<GLWindow*>(glfwGetWindowUserPointer(window));
		self->mWidth = width;
		self->mHeight = height;

		if (self->mFrameSizeCallback) {
			self->mFrameSizeCallback(self, width, height);
		}
	}


	void GLWindow::mouseMoveCallback(GLFWwindow* window, double xpos, double ypos) noexcept {
		auto self = static_cast<GLWindow*>(glfwGetWindowUserPointer(window));

		if (self->mMouseMoveCallback) {
			self->mMouseMoveCallback(xpos, ypos);
		}
	}

	void GLWindow::mouseActionCallback(GLFWwindow* window, int button, int action, int mods) noexcept {
		auto self = static_cast<GLFWwindow*>(glfwGetWindowUserPointer(window));

		MouseAction mouseAction{ MouseAction::NONE };

		auto iter = MouseActionMap.begin();
		while (iter != MouseActionMap.end()) {
			//iter->first 代表了哪个按键发生反应
			//iter->second拥有两个数据,第一个是down 第二个是up
			// 先检查按下还是抬起，之后根据那个按键find对应的tuple传给mMouseActionCallback
			if (action == GLFW_PRESS) {
				if (button == iter->first) {
					mouseAction = std::get<DOWN>(iter->second);
					break;
				}
			}
			else if (action == GLFW_RELEASE) {
				if (button == iter->first) {
					mouseAction = std::get<UP>(iter->second);
					break;
				}
			}
		}
	}

	bool GLWindow::processEvent() noexcept {

		if (glfwGetKey(mWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS || glfwWindowShouldClose(mWindow)) {
				glfwSetWindowShouldClose(mWindow, true);
				return false;
			}

			//在每一帧都会对事件进行集中的响应
			glfwPollEvents();

			//集中处理键盘事件
			bool needsNotify{ false };//键盘的某一个或者某几个状态只要发生了改变,都会置为true,键盘状态如果没有变化,不需要通知
			auto iter = KeyboardActionMap.begin();
			while (iter != KeyboardActionMap.end()) {
				//iter->first表示了glfw里面的某个按键
				//iter->second表示了我们对于这个按键的编号
				if (glfwGetKey(mWindow, iter->first) == GLFW_PRESS)
				{
					//不为1,表示之前没有记录按下的状态
					if (!mKeyState[iter->second]) {
						//set函数将当前这个位置的bit设置为1
						mKeyState.set(iter->second);
						needsNotify = true;
					}
				}
				else if (glfwGetKey(mWindow, iter->first) == GLFW_RELEASE) {
		
					if (mKeyState[iter->second]) {
				
						mKeyState.reset(iter->second);
						needsNotify = true;
					}
				}
				iter++;
			}

			if (mKeyboardActionCallback && needsNotify) {
				mKeyboardActionCallback(mKeyState);
			}

			return true;
		}


}