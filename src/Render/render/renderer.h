#pragma once
#include "../basic/common.h"
#include "../basic/constants.h"
#include "../camera/camera.h"
#include "../core/object3D.h"
#include "../objects/mesh.h"
#include "../scene/scene.h"
#include "renderTarget.h"
#include "GL/GLAttributes.h"
#include "GL/GLBindingStates.h"
#include "GL/GLPrograms.h"
#include "GL/GLWindow.h"
#include "GL/GLRenderList.h"
#include "GL/GLTextures.h"
#include "GL/GLObjects.h"
#include "GL/GLInfo.h"
#include "GL/GLState.h"
#include "GL/GLMaterials.h"
#include "GL/GLBackground.h"
#include "GL/GLRenderState.h"
#include "GL/GLRenderTargets.h"
#include "GL/GLShadowMap.h"
#include "../math/frustum.h"

namespace pe {

	class Renderer :public std::enable_shared_from_this<Renderer> {
	public:
		friend class GLShadowMap;

		struct Descriptor {
			uint32_t mWidth{ 800 };
			uint32_t mHeight{ 600 };
		};

		using OnSizeCallback = std::function<void(int width, int height)>;

		using Ptr = std::shared_ptr<Renderer>;
		static Ptr create(const Descriptor& descriptor) {
			return std::make_shared <Renderer>(descriptor);
		}

		Renderer(const Descriptor& descriptor) noexcept;

		~Renderer() noexcept;

		bool render(Scene::Ptr scene, Camera::Ptr camera);

		void swap() noexcept;

		// GLwindow
		void setSize(int width, int height) noexcept;

		void setRenderTarget(const RenderTarget::Ptr& renderTarget) noexcept;

		void setFrameSizeCallBack(const OnSizeCallback& callback) noexcept;

		void setMouseMoveCallBack(const GLWindow::MouseMoveCallback& callback) noexcept;

		void setMouseActionCallback(const GLWindow::MouseActionCallback& callback) noexcept;

		void setKeyboardActionCallBack(const GLWindow::KeyboardActionCallback& callback) noexcept;

		RenderTarget::Ptr getRenderTarget() const noexcept;

		void setClearColor(float r, float g, float b, float a) noexcept;

		glm::vec4 getClearColor() const noexcept;

		void enableShadow(bool enable) noexcept;

		void clear(bool color = true, bool depth = true, bool stencil = true) noexcept;

	public:
		//ÿһ�λ��ƣ��Ƿ���Ҫ�Զ�����ColorBuffer
		bool mAutoClear{ true };

	private:

		// 1 object ��ǰ��Ҫ��project��object��project�����㼶�ܹ�����֯��չ����Ϊ�б�
		// 2 groupOrder ��ǰ�������ڵ�group����Ⱦ���ȼ�
		// 3 sortObjects �Ƿ�����Ⱦ�б��У���item��������
		void projectObject(const Object3D::Ptr& object, uint32_t groupOrder, bool sortObjects) noexcept;

		//��һ�㼶���ڳ������𣬽���һЩ״̬�Ĵ��������ã����Ҹ���
		//ʵ��/͸��������ж�����Ⱦ-renderObjects
		void renderScene(const GLRenderList::Ptr& currentRenderList, const Scene::Ptr& scene, const Camera::Ptr& camera) noexcept;

		//�ڶ��㼶���ڶ��м��𣬽���һЩ״̬�Ĵ���������
		//���ε���ÿ����Ⱦ��Ԫ�����뵽renderObject
		void renderObjects(
			const std::vector<RenderItem::Ptr>& renderItems,
			const Scene::Ptr& scene,
			const Camera::Ptr& camera) noexcept;

		//�����㼶���ڵ�����Ⱦ��Ԫ�����ϣ�����һЩ״̬�Ĵ���������
		//���ҵ��ø�API������ص�renderBufferDirect
		void renderObject(
			const RenderableObject::Ptr& object,
			const Scene::Ptr& scene,
			const Camera::Ptr& camera,
			const Geometry::Ptr& geometry,
			const Material::Ptr& material) noexcept;

		void renderBufferDirect(
			const RenderableObject::Ptr& object,
			const Scene::Ptr& scene,
			const Camera::Ptr& camera,
			const Geometry::Ptr& geometry,
			const Material::Ptr& material) noexcept;

		GLProgram::Ptr setProgram(
			const Camera::Ptr& camera,
			const Scene::Ptr& scene,
			const Geometry::Ptr& geometry,
			const Material::Ptr& material,
			const RenderableObject::Ptr& object) noexcept;

		GLProgram::Ptr getProgram(
			const Material::Ptr& material,
			const Scene::Ptr& scene,
			const RenderableObject::Ptr& object) noexcept;

		void updateCommonMaterialProperties(
			const Material::Ptr& material,
			const GLProgram::Parameters::Ptr& parameters) noexcept;

		bool materialNeedsLights(const Material::Ptr& material) noexcept;

		void makeLightsNeedUpdate(UniformHandleMap& lightsUniformMap) noexcept;

	private:
		int mWidth{ 800 };
		int mHeight{ 600 };

		OnSizeCallback mOnSizeCallback{ nullptr };

		bool		mSortObject{ true };

		glm::mat4	mCurrentViewMatrix = glm::mat4(1.0f);

		glm::vec4	mViewport{};

		RenderTarget::Ptr	mCurrentRenderTarget{ nullptr };

		GLWindow::Ptr		mWindow{ nullptr };//done
		GLRenderList::Ptr	mRenderList{ nullptr };
		GLTextures::Ptr		mTextures{ nullptr };
		GLAttributes::Ptr   mAttributes{ nullptr }; //done
		GLInfo::Ptr			mInfos{ nullptr };//done
		GLState::Ptr		mState{ nullptr };
		GLObjects::Ptr		mObjects{ nullptr };//done
		GLGeometries::Ptr	mGeometries{ nullptr };
		GLPrograms::Ptr		mPrograms{ nullptr };
		GLBindingStates::Ptr	mBindingStates{ nullptr };//done
		GLMaterials::Ptr	mMaterials{ nullptr };
		GLBackground::Ptr	mBackground{ nullptr };
		GLRenderState::Ptr	mRenderState{ nullptr };
		GLRenderTargets::Ptr mRenderTargets{ nullptr };
		GLShadowMap::Ptr	mShadowMap{ nullptr };

		Frustum::Ptr			mFrustum{ nullptr };

		//dummy objects
		Scene::Ptr				mDummyScene = Scene::create();
	};
}