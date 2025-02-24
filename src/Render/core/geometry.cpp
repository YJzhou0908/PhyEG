#include "geometry.h"
#include "../tools/identity.h"

namespace pe {
	Geometry::Geometry() noexcept {
		mID = Identity::generateID();
	}

	Geometry::~Geometry() noexcept {
		EventBase::Ptr e = EventBase::create("geometryDispose");
		e->mTarget = this;
		EventDispatcher::getInstance()->dispatchEvent(e);
	}

	void Geometry::setAttribute(const std::string& name, Attributef::Ptr attribute) noexcept {
		// ��map�в���
		mAttributes[name] = attribute;
	}

	Attributef::Ptr Geometry::getAttribute(const std::string& name) noexcept {
		auto iter = mAttributes.find(name);
		if (iter != mAttributes.end()) {
			return iter->second;
		}
		return nullptr;
	}

	void Geometry::setIndex(const Attributei::Ptr& index) noexcept {
		mIndexAttribute = index;
	}

	void Geometry::deleteAttribute(const std::string& name) noexcept {
		auto iter = mAttributes.find(name);
		if (iter != mAttributes.end()) {
			mAttributes.erase(iter);
		}
		return;
	}

	bool Geometry::hasAttribute(const std::string& name) noexcept {
		auto iter = mAttributes.find(name);
		if (iter == mAttributes.end()) {
			return false;
		}

		return true;
	}

	const AttributeMap& Geometry::getAttributes() const noexcept {
		return mAttributes;
	}

	ID Geometry::getID() const noexcept {
		return mID;
	}

	void Geometry::computeBoundingBox() noexcept {
		// BoudingBox����AABB�Ľṹ
		// �鿴�Ƿ�����ÿ�������position Attribute
		auto position = getAttribute("position");

		if (position == nullptr) {
			std::cout << "ERROR : Geometry has no position attribute when compute bounding box." << std::endl;
		}

		// ����Ƿ��Ѿ���ʼ��
		if (mBoundingBox == nullptr) {
			mBoundingBox = Box3::create();
		}
		// �����Χ��
		mBoundingBox->setFromAttribute(position);
	
	}

	void Geometry::computeBoundingSphere() noexcept {
		// Ŀǰ�ò������Ȳ�д
	}
}