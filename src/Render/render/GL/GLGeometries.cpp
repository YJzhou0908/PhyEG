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

		//��Ϊһ��geometry���Ӧһ��GLBindState����һ��vao
		mBindingStates->releaseStatesOfGeometry(geometry->getID());

		mInfo->mMemory.mGeometries--;
	}

	//����һ�����ᣬ ������ÿһ��update��֮ǰ����geometry���������һ�θ���
	Geometry::Ptr GLGeometries::get(const Geometry::Ptr& geometry) noexcept {
		//geometries��һ��id-bool���͵�map��������¼��ǰ���geometry�Ƿ񱻼����infoһ��
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
			//ֻ�������indexAttribute֮���attributes 
			mAttributes->update(iter.second, BufferType::ArrayBuffer);
		}
	}
}