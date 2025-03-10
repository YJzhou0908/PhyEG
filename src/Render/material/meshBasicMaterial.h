#pragma once
#include "material.h"

namespace pe {

	class MeshBasicMaterial:public Material {
	public:
		using Ptr = std::shared_ptr<MeshBasicMaterial>;
		static Ptr create() { return std::make_shared<MeshBasicMaterial>(); }

		MeshBasicMaterial() noexcept;

		~MeshBasicMaterial() noexcept;
	};
}