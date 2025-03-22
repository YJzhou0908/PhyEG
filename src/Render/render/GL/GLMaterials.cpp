#include "GLMaterials.h"
#include "../../textures/texture.h"

namespace pe {

	GLMaterial::GLMaterial() noexcept {}

	GLMaterial::~GLMaterial() noexcept {}

	GLMaterials::GLMaterials(const GLPrograms::Ptr& programs) noexcept {
		mPrograms = programs;
		EventDispatcher::getInstance()->addEventListener("materialDispose", this, &GLMaterials::onMaterialDispose);
	}

	GLMaterials::~GLMaterials() noexcept {
		EventDispatcher::getInstance()->removeEventListener("materialDispose", this, &GLMaterials::onMaterialDispose);
	}

	GLMaterial::Ptr GLMaterials::get(const Material::Ptr& material) noexcept {
		auto iter = mMaterials.find(material->getID());

		if (iter == mMaterials.end()) {
			iter = mMaterials.insert(std::make_pair(material->getID(), GLMaterial::create())).first;
		}

		return iter->second;
	}

	void GLMaterials::onMaterialDispose(const EventBase::Ptr& event) {
		auto material = (Material*)event->mTarget;

		//比如我们生成了一个material但是并没有使用，然后就析构了
		auto iter = mMaterials.find(material->getID());
		if (iter == mMaterials.end()) {
			return;
		}

		auto dMaterial = iter->second;

		//拿到当前material曾经使用过的所有DriverPrograms, 并且放弃掉对他们的引用计数
		auto programs = dMaterial->mPrograms;
		for (const auto& pIter : programs) {
			auto program = pIter.second;

			//program是DriverProgram
			//mPrograms是DriverPrograms
			mPrograms->release(program);
		}

		mMaterials.erase(iter);
	}

	void GLMaterials::refreshMaterialUniforms(UniformHandleMap& uniformHandleMap, const Material::Ptr& material) {
		uniformHandleMap["opacity"].mValue = material->mOpacity;
		uniformHandleMap["opacity"].mNeedsUpdate = true;

		if (material->mIsMeshBasicMaterial) {
			auto basicMaterial = std::static_pointer_cast<MeshBasicMaterial>(material);
			refreshMaterialBasic(uniformHandleMap, basicMaterial);
		}

		if (material->mIsMeshPhongMaterial) {
			auto phongMaterial = std::static_pointer_cast<MeshPhongMaterial>(material);
			refreshMaterialPhong(uniformHandleMap, phongMaterial);
		}

		if (material->mIsCubeMaterial) {
			auto cubeMaterial = std::static_pointer_cast<CubeMaterial>(material);
			refreshMaterialCube(uniformHandleMap, cubeMaterial);
		}
	}

	void GLMaterials::refreshMaterialPhong(UniformHandleMap& uniformHandleMap, const MeshPhongMaterial::Ptr& material) {
		uniformHandleMap["shininess"].mValue = material->mShininess;
		uniformHandleMap["shininess"].mNeedsUpdate = true;

		if ((material->mDiffuseMap && material->mDiffuseMap->mNeedsUpdate) || material->mNeedsUpdate) {
			uniformHandleMap["diffuseMap"].mValue = material->mDiffuseMap;
			uniformHandleMap["diffuseMap"].mNeedsUpdate = true;
		}

		if ((material->mNormalMap && material->mNormalMap->mNeedsUpdate) || material->mNeedsUpdate) {
			uniformHandleMap["normalMap"].mValue = material->mNormalMap;
			uniformHandleMap["normalMap"].mNeedsUpdate = true;
		}

		if ((material->mSpecularMap && material->mSpecularMap->mNeedsUpdate) || material->mNeedsUpdate) {
			uniformHandleMap["specularMap"].mValue = material->mSpecularMap;
			uniformHandleMap["specularMap"].mNeedsUpdate = true;
		}
	}

	void GLMaterials::refreshMaterialBasic(UniformHandleMap& uniformHandleMap, const MeshBasicMaterial::Ptr& material) {
		if ((material->mDiffuseMap && material->mDiffuseMap->mNeedsUpdate) || material->mNeedsUpdate) {
			uniformHandleMap["diffuseMap"].mValue = material->mDiffuseMap;
			uniformHandleMap["diffuseMap"].mNeedsUpdate = true;
		}
	}

	void GLMaterials::refreshMaterialCube(UniformHandleMap& uniformHandleMap, const CubeMaterial::Ptr& material) {
		if ((material->mEnvMap && material->mEnvMap->mNeedsUpdate) || material->mNeedsUpdate) {
			uniformHandleMap["envMap"].mValue = material->mEnvMap;
			uniformHandleMap["envMap"].mNeedsUpdate = true;
		}
	}

}