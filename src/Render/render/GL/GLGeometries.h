#pragma once
#include "../../basic/common.h"
#include "../../basic/constants.h"
#include "../../basic/eventDispatcher.h"
#include "GLAttributes.h"
#include "GLInfo.h"
#include "GLBindingStates.h"

namespace pe {

	class GLGeometries {
	public:
		using Ptr = std::shared_ptr<GLGeometries>;
		static Ptr create(
			const GLAttributes::Ptr& attributes,
			const GLInfo::Ptr& info,
			const GLBindingStates::Ptr& bindingStates
		) {
			return std::make_shared <GLGeometries>(attributes, info, bindingStates);
		}

		GLGeometries(
			const GLAttributes::Ptr& attributes,
			const GLInfo::Ptr& info,
			const GLBindingStates::Ptr& bindingStates
		) noexcept;

		~GLGeometries() noexcept;

		Geometry::Ptr get(const Geometry::Ptr& geometry) noexcept;

		void onGeometryDispose(const EventBase::Ptr& event);

		void update(const Geometry::Ptr& geometry) noexcept;
	private:
		// 所有需要存储的VBO
		GLAttributes::Ptr mAttributes{ nullptr };
		// 所有的VAO
		GLBindingStates::Ptr mBindingStates{ nullptr };
		// 用来记录当前这个geometry是否被计算过info一次
		std::unordered_map<ID, bool> mGeometries{};

		GLInfo::Ptr mInfo{ nullptr };
	};


}
