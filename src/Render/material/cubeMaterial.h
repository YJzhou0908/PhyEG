#pragma once
#include "material.h"

namespace pe {

	class CubeMaterial :public Material {
	public:
		using Ptr = std::shared_ptr<CubeMaterial>;
		static Ptr create() { return std::make_shared<CubeMaterial>(); }

		CubeMaterial() noexcept;

		~CubeMaterial() noexcept;
	};
}