#include "depthMaterial.h"

namespace pe {

	DepthMaterial::DepthMaterial(uint32_t packing) noexcept {
		mType = MaterialName::DepthMaterial;
		mIsDepthMaterial = true;
		mDrawMode = DrawMode::Triangles;
		mPacking = packing;
	}

	DepthMaterial::~DepthMaterial() noexcept {}
}