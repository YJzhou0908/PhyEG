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

		//ǿ�Ƴ�����ֻ��һ�ֲ���
		Material::Ptr	mOverrideMaterial = nullptr;

		//��պ�
		CubeTexture::Ptr mBackground = nullptr;
	};
}