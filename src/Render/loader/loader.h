#pragma once
#include "../basic/common.h"
#include "../basic/constants.h"

namespace pe {

	class Loader {
	public:
		using Ptr = std::shared_ptr<Loader>;

		Loader() noexcept;

		~Loader() noexcept;

	};
}