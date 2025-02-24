#include "cubeMaterial.h"

namespace pe {

	CubeMaterial::CubeMaterial() noexcept {
		mType = MaterialName::CubeMaterial;
		mIsCubeMaterial = true;
		mDrawMode = DrawMode::Triangles;
	}

	CubeMaterial::~CubeMaterial() noexcept {}
}