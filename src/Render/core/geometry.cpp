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
		// 在map中插入
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

	const Geometry::AttributeMap& Geometry::getAttributes() const noexcept {
		return mAttributes;
	}

	ID Geometry::getID() const noexcept {
		return mID;
	}

	void Geometry::computeBoundingBox() noexcept {
		// BoudingBox采用AABB的结构
		// 查看是否设置每个顶点的position Attribute
		auto position = getAttribute("position");

		if (position == nullptr) {
			std::cout << "ERROR : Geometry has no position attribute when compute bounding box." << std::endl;
		}

		// 检查是否已经初始化
		if (mBoundingBox == nullptr) {
			mBoundingBox = Box3::create();
		}
		// 计算包围盒
		mBoundingBox->setFromAttribute(position);
	
	}

	void Geometry::computeBoundingSphere() noexcept {
		computeBoundingBox();
		if (mBoundingSphere == nullptr) {
			mBoundingSphere = Sphere::create(glm::vec3(0.0f), 0.0f);
		}

		//包围球跟包围盒共享了一个center
		mBoundingSphere->mCenter = mBoundingBox->getCenter();

		//find smallest sphere :inscribed sphere
		auto position = getAttribute("position");
		if (position == nullptr) {
			return;
		}

		//找到距离当前球心最大距离的点
		float maxRadiusSq = 0;
		for (uint32_t i = 0; i < position->getCount(); ++i) {
			//把每个顶点的xyz装成一个point
			glm::vec3 point = glm::vec3(position->getX(i), position->getY(i), position->getZ(i));

			//计算point到center的距离
			glm::vec3 radiusVector = mBoundingSphere->mCenter - point;

			//原本应该对比每一个点到center的距离，找到最大。但是计算向量长度，必须经过开方这个运算
			//为了性能考虑，直接记录其平方，最后得到最大值，再开二次方
			maxRadiusSq = std::max(glm::dot(radiusVector, radiusVector), maxRadiusSq);
		}

		//开方求取radius
		mBoundingSphere->mRadius = std::sqrt(maxRadiusSq);
	}
}