#include "meshPhongMaterial.h"

namespace pe {

	MeshPhongMaterial::MeshPhongMaterial() noexcept {
		mType = MaterialName::MeshPhongMaterial;
		mIsMeshPhongMaterial = true;
		mDrawMode = DrawMode::Triangles;
	}

	MeshPhongMaterial::~MeshPhongMaterial() noexcept {}
}