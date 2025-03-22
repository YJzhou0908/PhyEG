#pragma once 
#include "../../basic/common.h"
#include "../../basic/constants.h"
#include "GLObjects.h"
#include "GLRenderList.h"
#include "../../scene/scene.h"
#include "../../objects/mesh.h"

namespace pe {

	/*
	* 1 准备绘制天空盒的数据
	* 2 对渲染画布进行清理，对当前使用的RenderTarget的ColorBuffer进行清理
	*/
	class Renderer;
	class GLBackground {
	public:
		using Ptr = std::shared_ptr<GLBackground>;
		static Ptr create(Renderer* renderer, const GLObjects::Ptr& objects) {
			return std::make_shared<GLBackground>(renderer, objects);
		}

		GLBackground(Renderer* renderer, const GLObjects::Ptr& objects) noexcept;

		~GLBackground() noexcept;

		void render(const GLRenderList::Ptr& renderList, const Scene::Ptr& scene);

	private:
		Renderer* mRenderer{ nullptr };
		GLObjects::Ptr mObjects{ nullptr };

		Mesh::Ptr	mBoxMesh{ nullptr };
	};
}