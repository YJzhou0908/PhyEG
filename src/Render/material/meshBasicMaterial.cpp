#include "meshBasicMaterial.h"

namespace pe {

	MeshBasicMaterial::MeshBasicMaterial() noexcept {
		mType = MaterialName::MeshBasicMaterial;
		mIsMeshBasicMaterial = true;
		mDrawMode = DrawMode::Triangles;
	}

	MeshBasicMaterial::~MeshBasicMaterial() noexcept {}
}