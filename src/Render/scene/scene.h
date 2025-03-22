#pragma once 
#include "../basic/common.h"
#include "../core/object3D.h"
#include "../material/material.h"
#include "../textures/cubeTexture.h"

namespace pe {
	class Scene : public Object3D {
	public:
		using Ptr = std::shared_ptr<Scene>;

		static Ptr create() {
			return std::make_shared<Scene>();
		}


		Scene() noexcept;

		~Scene() noexcept;

		//强制场景内只有一种材质
		Material::Ptr	mOverrideMaterial = nullptr;

		//天空盒
		CubeTexture::Ptr mBackground = nullptr;
	};
}