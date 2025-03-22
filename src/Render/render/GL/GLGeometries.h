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
		// ������Ҫ�洢��VBO
		GLAttributes::Ptr mAttributes{ nullptr };
		// ���е�VAO
		GLBindingStates::Ptr mBindingStates{ nullptr };
		// ������¼��ǰ���geometry�Ƿ񱻼����infoһ��
		std::unordered_map<ID, bool> mGeometries{};

		GLInfo::Ptr mInfo{ nullptr };
	};


}
