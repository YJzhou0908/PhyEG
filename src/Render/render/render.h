#pragma once
#include "../basic/common.h"
#include "../basic/constants.h"
#include "../core/object3D.h"
#include "../objects/mesh.h"
#include "../scene/scene.h"


namespace pe {
	/*
	Renderer �Ǵ����ǰ�ˣ��߱����������˹�ͨ��API��ǰ������ѡ��
	*/
	
	class Renderer :public enable_shared_from_this<Renderer> {
	public:

		using OnSizeCallBack = std::function<void(int , int )>; // ������ߴ緢���仯ʱ

		// ������չ��Ⱦ����������Ϣ
		struct Descriptor {
			uint32_t mWidth{ 800 };
			uint32_t mHeight{ 600 };
		};

		using Ptr = std::shared_ptr<Renderer>;
		static Ptr create(const Descriptor& dc) {
			return std::make_shared<Renderer>(dc);
		}

		Renderer(const Descriptor& dc) noexcept;

		~Renderer() noexcept;

		bool render(Scene::Ptr& scene, Camera::Ptr& camera);

		void setSize(int width, int height) noexcept;

		void setClearColor(float r, float g, float b, float a) noexcept;

		glm::vec4 getClearColor() const noexcept;

		void clear(bool color = true, bool depth = true, bool stencil = true) noexcept;
	public:
		//ÿһ�λ��ƣ��Ƿ���Ҫ�Զ�����ColorBuffer
		bool mAutoClear{ true };

	private:
		GLWindow::Ptr mGLwindow;


	private:
		int mWidth{ 800 };
		int mHeight{ 600 };
		OnSizeCallback mOnSizeCallback{ nullptr };

		bool		mSortObject{ true };

		glm::mat4	mCurrentViewMatrix = glm::mat4(1.0f);

		glm::vec4	mViewport{};

		//dummy objects
		Scene::Ptr				mDummyScene = Scene::create();

	
	};


}