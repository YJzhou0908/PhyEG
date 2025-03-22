#pragma once
#include "../../basic/common.h"
#include "../../objects/renderableObject.h"
#include "GLGeometries.h"
#include "GLAttributes.h"
#include "GLInfo.h"

namespace pe {

	// 上层操前端RenderableObject对应的后端，包括了后端的GLGeometry和GLAttributes
	class GLObjects {
	public:
		using Ptr = std::shared_ptr<GLObjects>;
		static Ptr create(
			const GLGeometries::Ptr& geometries,
			const GLAttributes::Ptr& attributes,
			const GLInfo::Ptr& info
		) {
			return std::make_shared <GLObjects>(geometries, attributes, info);
		}
		// 需要VAO和VBO才能构建Objects
		GLObjects(
			const GLGeometries::Ptr& geometries,
			const GLAttributes::Ptr& attributes,
			const GLInfo::Ptr& info
		) noexcept;

		~GLObjects() noexcept;

		// 根据前端构建的渲染对象，更新后端
		Geometry::Ptr update(const RenderableObject::Ptr& object) noexcept; 

	private:
		// 前面是对象ID，后面是
		std::unordered_map<ID, uint32_t> mUpdateMap{};

		GLInfo::Ptr mInfo{ nullptr };
		GLAttributes::Ptr mAttributes{ nullptr };
		GLGeometries::Ptr mGeometries{ nullptr };
	};
}