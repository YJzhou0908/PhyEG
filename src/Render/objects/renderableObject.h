#pragma once
#include "../basic/common.h"
#include "../basic/constants.h"
#include "../core/object3D.h"
#include "../material/material.h"
#include "../core/geometry.h"


namespace pe {

	class Renderer;
	class Scene;
	class Camera;

	class RenderableObject :public Object3D {
	public:
		using Ptr = std::shared_ptr<RenderableObject>;
		
		using OnBeforeRenderCallback = std::function<void(Renderer* , Scene* , Camera* )>;

		RenderableObject(const Geometry::Ptr& geometry, const Material::Ptr& material) noexcept;

		~RenderableObject() noexcept;

		auto getGeometry() const noexcept { return mGeometry; }

		auto getMaterial() const noexcept { return mMaterial; }

		void onBeforeRender(Renderer* renderer, Scene* scene, Camera* camera);

	public:
		OnBeforeRenderCallback mOnBeforeRenderCallback{ nullptr };

	protected:
		Geometry::Ptr mGeometry{ nullptr };
		Material::Ptr mMaterial{ nullptr };
	};



}