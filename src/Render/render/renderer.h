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
		//每一次绘制，是否需要自动擦除ColorBuffer
		bool mAutoClear{ true };

	private:

		// 1 object 当前需要被project的object，project：将层级架构的组织，展开成为列表
		// 2 groupOrder 当前其所处于的group的渲染优先级
		// 3 sortObjects 是否在渲染列表中，对item进行排序
		void projectObject(const Object3D::Ptr& object, uint32_t groupOrder, bool sortObjects) noexcept;

		//第一层级，在场景级别，进行一些状态的处理与设置，并且根据
		//实体/透明物体进行队列渲染-renderObjects
		void renderScene(const GLRenderList::Ptr& currentRenderList, const Scene::Ptr& scene, const Camera::Ptr& camera) noexcept;

		//第二层级，在队列级别，进行一些状态的处理与设置
		//依次调用每个渲染单元，进入到renderObject
		void renderObjects(
			const std::vector<RenderItem::Ptr>& renderItems,
			const Scene::Ptr& scene,
			const Camera::Ptr& camera) noexcept;

		//第三层级，在单个渲染单元层面上，进行一些状态的处理与设置
		//并且调用跟API相关深重的renderBufferDirect
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