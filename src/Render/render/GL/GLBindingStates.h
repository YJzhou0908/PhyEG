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

		//跟cache校验有关
		//存储了attribute的名字字符串作为key，attribute对象的id做为value
		std::unordered_map<std::string, ID> mAttributes{};

		//记录了对应的geometry的indexAttribute的id
		ID mIndex{ 0 };

		//记录了总共有多少个Attribute
		uint32_t mAttributesNum{ 0 };
	};

	//一个VAO与一个Geometry一一对应
	class GLBindingStates {
	public:
		//key:geometry的ID号  value：BindingState这里面蕴含着一个VAO
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