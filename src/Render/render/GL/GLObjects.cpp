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
		//1 拿到当前到了第几帧
		const auto frame = mInfo->mRender.mFrame;

		//2 拿出geometry，并且在get里面做相关的数据记录
		const auto geometry = mGeometries->get(object->getGeometry());

		//避免重复的Geometry被渲染
		//key：geometry的ID
		//value：frameNumber
		auto iter = mUpdateMap.find(geometry->getID());

		if (iter == mUpdateMap.end() || mUpdateMap[geometry->getID()] != frame) {
			mGeometries->update(geometry);
			mUpdateMap[geometry->getID()] = frame;
		}

		return geometry;
	}
}