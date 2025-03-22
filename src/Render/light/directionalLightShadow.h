#pragma once 
#include "lightShadow.h"

namespace pe {

	class DirectionalLightShadow :public LightShadow {
	public:
		using Ptr = std::shared_ptr<DirectionalLightShadow>;
		static Ptr create() {
			return std::make_shared<DirectionalLightShadow>();
		}

		DirectionalLightShadow()noexcept;

		~DirectionalLightShadow()noexcept;

	};
}