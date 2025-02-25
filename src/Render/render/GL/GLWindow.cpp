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

		// ��������
		mWindow = glfwCreateWindow(mWidth, mHeight, "Window", nullptr, nullptr);

		if (!mWindow) {
			std::cerr << "Error: failed to create window" << std::endl;
			glfwTerminate();
			exit(0);
		}

		glfwMakeContextCurrent(mWindow);

		// ����OpenGL��API
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			std::cout << "Failed to initialize GLAD" << std::endl;
			exit(0);
		}

		//����ǰ��GLWindow�����Ķ����thisָ�룬��Ϊ��һ��userData��������GLFWWindow��mWindow��
		glfwSetWindowUserPointer(mWindow, this);

		//ע��ص�����
		glfwSetFramebufferSizeCallback(mWindow, frameSizeCallback);

		glfwSetMouseButtonCallback(mWindow, mouseActionCallback);

		glfwSetCursorPosCallback(mWindow, mouseMoveCallback);

		//close the v-sync for screen 
		// ��ֱͬ���Ĺرգ���ʾ����һ����ˢ��Ƶ�ʣ�����ʱ���Ⱦ�ٶ�Ҫ���ܶ࣬����
		// ��ֱͬ����ʱ����Ⱦ�ٶȻ���ˢ��Ƶ��һ��
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


	// ����ע��Ļص�����
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
			//iter->first �������ĸ�����������Ӧ
			//iter->secondӵ����������,��һ����down �ڶ�����up
			// �ȼ�鰴�»���̧��֮������Ǹ�����find��Ӧ��tuple����mMouseActionCallback
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

			//��ÿһ֡������¼����м��е���Ӧ
			glfwPollEvents();

			//���д�������¼�
			bool needsNotify{ false };//���̵�ĳһ������ĳ����״ֻ̬Ҫ�����˸ı�,������Ϊtrue,����״̬���û�б仯,����Ҫ֪ͨ
			auto iter = KeyboardActionMap.begin();
			while (iter != KeyboardActionMap.end()) {
				//iter->first��ʾ��glfw�����ĳ������
				//iter->second��ʾ�����Ƕ�����������ı��
				if (glfwGetKey(mWindow, iter->first) == GLFW_PRESS)
				{
					//��Ϊ1,��ʾ֮ǰû�м�¼���µ�״̬
					if (!mKeyState[iter->second]) {
						//set��������ǰ���λ�õ�bit����Ϊ1
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