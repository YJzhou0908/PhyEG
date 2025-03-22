#pragma once
#include "../../basic/common.h"
#include "../../camera/camera.h"
#include "../../scene/scene.h"
#include "../../light/light.h"
#include "../../material/depthMaterial.h"

namespace pe {

	class GLRenderState;
	class GLObjects;
	class Renderer;
	class GLState;
	class GLShadowMap {
	public:
		using Ptr = std::shared_ptr<GLShadowMap>;
		static Ptr create(Renderer* renderer, const std::shared_ptr<GLObjects>& objects, const std::shared_ptr<GLState>& state) {
			return std::make_shared<GLShadowMap>(renderer, objects, state);
		}

		GLShadowMap(Renderer* renderer, const std::shared_ptr<GLObjects>& objects, const std::shared_ptr<GLState>& state) noexcept;

		~GLShadowMap() noexcept;

		void render(const std::shared_ptr<GLRenderState>& renderState, const Scene::Ptr& scene, const Camera::Ptr& camera) noexcept;

		void renderObject(
			const Object3D::Ptr& object,
			const Camera::Ptr& camera,
			const Camera::Ptr& shadowCamera,
			const Light::Ptr& light,
			const Frustum::Ptr& frustum) noexcept;

	public:
		//决定整个系统是否开启ShadowMap
		bool mEnabled{ true };

	private:
		Renderer* mRenderer{ nullptr };
		std::shared_ptr<GLObjects>	mObjects{ nullptr };
		std::shared_ptr<GLState>	mState{ nullptr };

		DepthMaterial::Ptr	mDefaultDepthMaterial = DepthMaterial::create(DepthMaterial::RGBADepthPacking);
	};
}