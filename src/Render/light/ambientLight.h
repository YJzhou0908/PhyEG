#pragma once
#include "light.h"

namespace pe {

	class AmbientLight:public Light {
	public:
		using Ptr = std::shared_ptr<AmbientLight>;
		static Ptr create() {
			return std::make_shared<AmbientLight>();
		}

		AmbientLight() noexcept;

		~AmbientLight() noexcept;
	};
}