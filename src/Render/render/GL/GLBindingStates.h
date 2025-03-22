#pragma once
#include "../../basic/common.h"
#include "../../basic/constants.h"
#include "../../core/geometry.h"
#include "../../core/object3D.h"
#include "../../core/attribute.h"
#include "../../material/material.h"
#include "GLAttributes.h"
#include "GLPrograms.h"

namespace pe {

	class GLBindingStates;
	class GLBindingState {
		friend GLBindingStates;
	public:
		using Ptr = std::shared_ptr<GLBindingState>;
		static Ptr create() {
			return std::make_shared <GLBindingState>();
		}

		GLBindingState() noexcept;

		~GLBindingState() noexcept;

	private:
		GLuint	mVAO{ 0 };

		//��cacheУ���й�
		//�洢��attribute�������ַ�����Ϊkey��attribute�����id��Ϊvalue
		std::unordered_map<std::string, ID> mAttributes{};

		//��¼�˶�Ӧ��geometry��indexAttribute��id
		ID mIndex{ 0 };

		//��¼���ܹ��ж��ٸ�Attribute
		uint32_t mAttributesNum{ 0 };
	};

	//һ��VAO��һ��Geometryһһ��Ӧ
	class GLBindingStates {
	public:
		//key:geometry��ID��  value��BindingState�������̺���һ��VAO
		using GeometryKeyMap = std::unordered_map<ID, GLBindingState::Ptr>;

		using Ptr = std::shared_ptr<GLBindingStates>;
		static Ptr create(const GLAttributes::Ptr& attributes) {
			return std::make_shared <GLBindingStates>(attributes);
		}

		GLBindingStates(const GLAttributes::Ptr& attributes);

		~GLBindingStates();

		GLBindingState::Ptr getBindingState(const Geometry::Ptr& geometry) noexcept;

		void setup(
			const Geometry::Ptr& geometry,
			const Attributei::Ptr& index);

		GLBindingState::Ptr createBindingState(GLuint vao) noexcept;

		bool needsUpdate(const Geometry::Ptr& geometry, const Attributei::Ptr& index) noexcept;

		void saveCache(const Geometry::Ptr& geometry, const Attributei::Ptr& index) noexcept;

		void setupVertexAttributes(const Geometry::Ptr& geometry) noexcept;

		GLuint createVAO() noexcept;

		void bindVAO(GLuint vao) noexcept;

		void releaseStatesOfGeometry(ID geometryID) noexcept;

	private:
		GLAttributes::Ptr	mAttributes{ nullptr };
		GLBindingState::Ptr mCurrentBindingState{ nullptr };
		GeometryKeyMap	mBindingStates{};
	};
}