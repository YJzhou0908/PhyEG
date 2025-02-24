#pragma once
#include "../basic/common.h"
#include "../basic/constants.h"
#include "../core/object3D.h"

namespace pe {

	class Light :public Objec3D {
	public:
		using Ptr = std::shared_ptr<Light>;
		static Ptr create() {
			return std::make_shared<Light>();
		}

		Light() noexcept;

		~Light() noexcept;

	public:
		glm::vec3	mColor = glm::vec3(1.0f);
		float		mIntensity = 1.0f;
		bool		mCastShadow{ false };//是否产生阴影
		LightShadow::Ptr	mShadow{ nullptr };
	};


}