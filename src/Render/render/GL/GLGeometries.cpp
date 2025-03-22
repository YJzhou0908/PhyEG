#include "GLGeometries.h"
#include "../../basic/eventDispatcher.h"

namespace pe {

	GLGeometries::GLGeometries(
		const GLAttributes::Ptr& attributes,
		const GLInfo::Ptr& info,
		const GLBindingStates::Ptr& bindingStates
	) noexcept {
		mAttributes = attributes;
		mInfo = info;
		mBindingStates = bindingStates;

		EventDispatcher::getInstance()->addEventListener("geometryDispose", this, &GLGeometries::onGeometryDispose);
	}

	GLGeometries::~GLGeometries() noexcept {
		EventDispatcher::getInstance()->removeEventListener("geometryDispose", this, &GLGeometries::onGeometryDispose);
	}

	void GLGeometries::onGeometryDispose(const EventBase::Ptr& event) {
		const auto geometry = static_cast<Geometry*>(event->mTarget);

		mGeometries.erase(geometry->getID());

		//因为一个geometry会对应一个GLBindState，即一个vao
		mBindingStates->releaseStatesOfGeometry(geometry->getID());

		mInfo->mMemory.mGeometries--;
	}

	//给了一个机会， 可以在每一次update的之前，对geometry相关数据做一次更新
	Geometry::Ptr GLGeometries::get(const Geometry::Ptr& geometry) noexcept {
		//geometries是一个id-bool类型的map，用来记录当前这个geometry是否被计算过info一次
		auto iter = mGeometries.find(geometry->getID());
		if (iter != mGeometries.end()) {
			if (iter->second == true) {
				return geometry;
			}
		}
		mGeometries[geometry->getID()] = true;

		mInfo->mMemory.mGeometries++;

		return geometry;
	}

	void GLGeometries::update(const Geometry::Ptr& geometry) noexcept {
		const auto geometryAttributes = geometry->getAttributes();
		for (const auto& iter : geometryAttributes) {
			//只处理除了indexAttribute之外的attributes 
			mAttributes->update(iter.second, BufferType::ArrayBuffer);
		}
	}
}