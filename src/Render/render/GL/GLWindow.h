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

		//���ø����¼��Ļص�����
		void setFrameSizeCallBack(const FrameSizeCallback& callback) noexcept;

		void setMouseMoveCallBack(const MouseMoveCallback& callback) noexcept;

		void setMouseActionCallback(const MouseActionCallback& callback) noexcept;

		void setKeyboardActionCallBack(const KeyboardActionCallback& callback) noexcept;


		//ÿһ֡��Ⱦǰ���������processEvent�����¼�������Ӧ
		bool processEvent() noexcept;

		//˫����ĸ���
		void swap() noexcept;

		GLFWwindow* getWindow() const  noexcept { return mWindow; }

	private:
		// ˽�л�glfw����¼������Ȼص��·��������������Ѿ�����windowע��Ľӿڣ���ô���·������л�������ص�����
		static void frameSizeCallback(GLFWwindow* window, int width, int height) noexcept;

		static void mouseMoveCallback(GLFWwindow* window, double xpos, double ypos) noexcept;

		static void mouseActionCallback(GLFWwindow* window, int button, int action, int mods) noexcept;

	public:
		Renderer* mRender{ nullptr };

	private:
		int mWidth{ 0 };
		int mHeight{ 0 };

		KeyBoardState mKeyState{};

		//�������������ػص�����
		FrameSizeCallback		mFrameSizeCallback{ nullptr };
		MouseMoveCallback		mMouseMoveCallback{ nullptr };
		KeyboardActionCallback	mKeyboardActionCallback{ nullptr };
		MouseActionCallback		mMouseActionCallback{ nullptr };

		GLFWwindow* mWindow{ NULL };
	
	};

}
