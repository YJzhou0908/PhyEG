#pragma once 
#include "../basic/common.h"
#include "attribute.h"
#include "../math/box3.h"
#include "../math/sphere.h"
#include "../basic/eventDispatcher.h"

namespace pe {
	// 对应VAO ， GLGeometry
	class Geometry : public std::enable_shared_from_this<Geometry> {
	public:
		// 通过字符串可以索引到对应的Attribute的前端。
		using AttributeMap = std::unordered_map<std::string, Attributef::Ptr>;

		using Ptr = std::shared_ptr<Geometry>;
		static Ptr create() {
			return std::make_shared <Geometry>();
		}

		Geometry() noexcept;

		~Geometry() noexcept;

		void setAttribute(const std::string& name, Attributef::Ptr) noexcept;

		Attributef::Ptr getAttribute(const std::string& name) noexcept;

		void setIndex(const Attributei::Ptr& index) noexcept;

		void deleteAttribute(const std::string& name) noexcept;

		bool hasAttribute(const std::string& name) noexcept;

		const AttributeMap& getAttributes() const noexcept;

		ID getID() const noexcept;

		auto getIndex() const noexcept { return mIndexAttribute; }

		void computeBoundingBox() noexcept;

		void computeBoundingSphere() noexcept;

		Sphere::Ptr getBoundingSphere() const noexcept { return mBoundingSphere; }

		Box3::Ptr getBoundingBox() const noexcept { return mBoundingBox; }

	private:
		ID mID{ 0 };

		AttributeMap mAttributes{};

		Attributei::Ptr mIndexAttribute{ nullptr };

		Box3::Ptr	mBoundingBox{ nullptr }; 

		Sphere::Ptr	mBoundingSphere{ nullptr }; 
	};
}
