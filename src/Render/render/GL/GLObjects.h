#pragma once
#include "../../basic/common.h"
#include "../../objects/renderableObject.h"
#include "GLGeometries.h"
#include "GLAttributes.h"
#include "GLInfo.h"

namespace pe {

	// �ϲ��ǰ��RenderableObject��Ӧ�ĺ�ˣ������˺�˵�GLGeometry��GLAttributes
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
		// ��ҪVAO��VBO���ܹ���Objects
		GLObjects(
			const GLGeometries::Ptr& geometries,
			const GLAttributes::Ptr& attributes,
			const GLInfo::Ptr& info
		) noexcept;

		~GLObjects() noexcept;

		// ����ǰ�˹�������Ⱦ���󣬸��º��
		Geometry::Ptr update(const RenderableObject::Ptr& object) noexcept; 

	private:
		// ǰ���Ƕ���ID��������
		std::unordered_map<ID, uint32_t> mUpdateMap{};

		GLInfo::Ptr mInfo{ nullptr };
		GLAttributes::Ptr mAttributes{ nullptr };
		GLGeometries::Ptr mGeometries{ nullptr };
	};
}