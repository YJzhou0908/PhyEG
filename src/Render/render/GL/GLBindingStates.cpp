#include "GLBindingStates.h"
#include "../../core/geometry.h"

namespace pe {

	GLBindingState::GLBindingState() noexcept {}

	GLBindingState::~GLBindingState() noexcept {
		if (mVAO) {
			glDeleteVertexArrays(1, &mVAO);
		}
	}

	GLBindingStates::GLBindingStates(const GLAttributes::Ptr& attributes) {
		mAttributes = attributes;
	}

	GLBindingStates::~GLBindingStates() {}

	GLBindingState::Ptr GLBindingStates::getBindingState(const Geometry::Ptr& geometry) noexcept {
		GLBindingState::Ptr state = nullptr;

		auto gKeyIter = mBindingStates.find(geometry->getID());
		if (gKeyIter == mBindingStates.end()) {
			gKeyIter = mBindingStates.insert(std::make_pair(geometry->getID(), createBindingState(createVAO()))).first;
		}

		state = gKeyIter->second;

		return state;
	}

	void GLBindingStates::setup(
		const Geometry::Ptr& geometry,
		const Attributei::Ptr& index
	) {
		bool updateBufferLayout = false;

		auto state = getBindingState(geometry);
		if (mCurrentBindingState != state) {
			mCurrentBindingState = state;
			bindVAO(state->mVAO);
		}

		updateBufferLayout = needsUpdate(geometry, index);
		if (updateBufferLayout) {
			saveCache(geometry, index);
		}

		//index应该与vao平级处理
		if (index != nullptr) {
			mAttributes->update(index, BufferType::IndexBuffer);
		}

		if (updateBufferLayout) {
			setupVertexAttributes(geometry);

			if (index != nullptr) {
				auto bkIndex = mAttributes->get(index);
				if (bkIndex != nullptr) {
					glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bkIndex->mHandle);
				}

			}
		}
	}

	GLBindingState::Ptr GLBindingStates::createBindingState(GLuint vao) noexcept {
		auto bindingState = GLBindingState::create();
		bindingState->mVAO = vao;

		return bindingState;
	}

	bool GLBindingStates::needsUpdate(const Geometry::Ptr& geometry, const Attributei::Ptr& index) noexcept {
		//id->名字，value->attribute id
		auto cachedAttributes = mCurrentBindingState->mAttributes;

		//id->名字，value->attribute对象
		auto geometryAttributes = geometry->getAttributes();

		uint32_t attributesNum = 0;
		for (const auto& iter : geometryAttributes) {
			auto key = iter.first;
			auto geometryAttribute = iter.second;

			//从缓存里面寻找，但凡有一个attribute没找到，说明就不一样了
			auto cachedIter = cachedAttributes.find(key);
			if (cachedIter == cachedAttributes.end()) {
				return true;
			}

			//从缓存当中，确实找到了这个attribute，那么就得对比id是否一致了
			//从而确定，同样名字的attribute是否是同一个
			auto cachedAttribute = cachedIter->second;
			if (cachedAttribute != geometryAttribute->getID()) {
				return true;
			}

			attributesNum++;
		}

		if (mCurrentBindingState->mAttributesNum != attributesNum) {
			return true;
		}

		//indexAttribute 如果不同，仍然需要重新挂钩
		if (index != nullptr && mCurrentBindingState->mIndex != index->getID()) {
			return true;
		}

		//如果上述结果都一致，则返回不需要重新挂勾
		return false;
	}

	void GLBindingStates::saveCache(const Geometry::Ptr& geometry, const Attributei::Ptr& index) noexcept {
		//首先清空掉bindingState里面的attributes （Map）
		auto& cachedAttributes = mCurrentBindingState->mAttributes;
		cachedAttributes.clear();

		auto attributes = geometry->getAttributes();
		uint32_t attributesNum = 0;

		//遍历geometry的每一个attribute
		for (const auto& iter : attributes) {
			auto attribute = iter.second;

			//插入key-attributeName，value-attributeID
			cachedAttributes.insert(std::make_pair(iter.first, attribute->getID()));
			attributesNum++;
		}

		mCurrentBindingState->mAttributesNum = attributesNum;

		if (index != nullptr) {
			mCurrentBindingState->mIndex = index->getID();
		}
	}

	void GLBindingStates::setupVertexAttributes(const Geometry::Ptr& geometry) noexcept {
		const auto geometryAttributes = geometry->getAttributes();

		for (const auto& iter : geometryAttributes) {
			auto name = iter.first;
			auto attribute = iter.second;

			//itemSize本attribute有多少个数字，比如position就是3个数字
			auto itemSize = attribute->getItemSize();

			//每个单独的数据的类型
			auto dataType = attribute->getDataType();

			//将attribute对应的DriverAttribute取出来
			auto bkAttribute = mAttributes->get(attribute);

			//将本attribute的location(binding)通过attribute的name取出来
			auto bindingIter = LOCATION_MAP.find(name);
			if (bindingIter == LOCATION_MAP.end()) {
				continue;
			}

			auto binding = bindingIter->second;

			//开始向vao里面做挂钩关系
			glBindBuffer(GL_ARRAY_BUFFER, bkAttribute->mHandle);
			//激活对应的binding点
			glEnableVertexAttribArray(binding);
			//向vao里面记录，对于本binding点所对应的attribute，我们应该如何从bkAttribute->mHandle一个vbo里面读取数据
			glVertexAttribPointer(binding, itemSize, toGL(dataType), false, itemSize * toSize(dataType), (void*)0);
		}
	}

	//真正的生成了一个VAO
	GLuint GLBindingStates::createVAO() noexcept {
		GLuint vao = 0;
		glGenVertexArrays(1, &vao);
		return vao;
	}

	// 真正改变了OpenGL状态机，使之绑定当前的vao
	void GLBindingStates::bindVAO(GLuint vao) noexcept {
		glBindVertexArray(vao);
	}

	void GLBindingStates::releaseStatesOfGeometry(ID geometryID) noexcept {
		auto iter = mBindingStates.find(geometryID);
		if (iter != mBindingStates.end()) {
			mBindingStates.erase(iter);
		}
	}

}