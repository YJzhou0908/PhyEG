#include "GLObjects.h"
#include "../../basic/eventDispatcher.h"

namespace pe {
	GLObjects::GLObjects(
		const GLGeometries::Ptr& geometries,
		const GLAttributes::Ptr& attributes,
		const GLInfo::Ptr& info
	) noexcept {
		mGeometries = geometries;
		mAttributes = attributes;
		mInfo = info;
	}

	GLObjects::~GLObjects() {
	
	}
	
	Geometry::Ptr GLObjects::update(const RenderableObject::Ptr& object) noexcept {
		//1 �õ���ǰ���˵ڼ�֡
		const auto frame = mInfo->mRender.mFrame;

		//2 �ó�geometry��������get��������ص����ݼ�¼
		const auto geometry = mGeometries->get(object->getGeometry());

		//�����ظ���Geometry����Ⱦ
		//key��geometry��ID
		//value��frameNumber
		auto iter = mUpdateMap.find(geometry->getID());

		if (iter == mUpdateMap.end() || mUpdateMap[geometry->getID()] != frame) {
			mGeometries->update(geometry);
			mUpdateMap[geometry->getID()] = frame;
		}

		return geometry;
	}
}