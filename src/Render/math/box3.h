#pragma once 
#include "../../basic/common.h"
#include "../../core/attribute.h"
namespace pe {

	class Box3 {
	public:
		using Ptr = std::shared_ptr<Box3>;
		static Ptr create() {
			return std::make_shared<Box3>();
		}

		Box3() noexcept {}

		~Box3() noexcept {}

		// 对Geometry设置最大包围盒 AABB型
		void setFromAttribute(const Attributef::Ptr& attribute) noexcept {
			for (uint32_t i = 0; i < attribute->getCount(); ++i) {
				float x = attribute->getX(i);
				float y = attribute->getY(i);
				float z = attribute->getZ(i);

				if (x < mMin.x) { mMin.x = x; }
				if (y < mMin.y) { mMin.y = y; }
				if (z < mMin.z) { mMin.z = z; }

				if (x > mMax.x) { mMax.x = x; }
				if (y > mMax.y) { mMax.y = y; }
				if (z > mMax.z) { mMax.z = z; }
			}
		}

		bool isEmpty() noexcept {
			return (mMax.x < mMin.x || mMax.y < mMin.y || mMax.z < mMin.z);
		}

		glm::vec3 getCenter() noexcept {
			if (isEmpty()) {
				return glm::vec3(0.0f);
			}
			return (mMax + mMin) / 2.0f;
		}

	public:
		glm::vec3 mMin = glm::vec3(std::numeric_limits<float>::infinity());
		glm::vec3 mMax = glm::vec3(-std::numeric_limits<float>::infinity());
	};
}