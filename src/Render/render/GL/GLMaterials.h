#pragma once
#include "../../basic/common.h"
#include "../../basic/constants.h"
#include "../../basic/eventDispatcher.h"
#include "../../textures/texture.h"
#include "../../material/material.h"
#include "../../material/cubeMaterial.h"
#include "../../material/meshBasicMaterial.h"
#include "../../material/meshPhongMaterial.h"
#include "GLPrograms.h"
#include "GLUniforms.h"
#include "GLTextures.h"
#include "../shaders/uniforms.h"

namespace pe {

	//1 ��¼�˶�Ӧ��ǰ�˵�Material�Ĳ���
	//2 ��¼�˶�Ӧ��ǰ�˵�material����һ��ʼ����ǰ��ʹ�ù������е�Program
	class GLMaterial {
	public:
		using Ptr = std::shared_ptr<GLMaterial>;
		static Ptr create() { return std::make_shared<GLMaterial>(); }

		GLMaterial() noexcept;

		~GLMaterial() noexcept;

	public:
		uint32_t				mVersion{ 0 };
		bool					mInstancing{ false };
		GLProgram::Ptr		mCurrentProgram{ nullptr };

		Texture::Ptr			mDiffuseMap{ nullptr };
		Texture::Ptr			mEnvMap{ nullptr };
		Texture::Ptr			mNormalMap{ nullptr };
		Texture::Ptr			mSpecularMap{ nullptr };

		bool					mNeedsLight{ nullptr };
		uint32_t				mLightsStateVersion{ 0 };

		bool					mSkinning{ false };
		uint32_t				mMaxBones{ 0 };

		//��¼��ǰ�˶�Ӧ��material��ʹ�ù���
		std::unordered_map<HashType, GLProgram::Ptr> mPrograms{};

		UniformHandleMap		mUniforms{};
	};

	class GLMaterials {
	public:
		using Ptr = std::shared_ptr<GLMaterials>;
		static Ptr create(const GLPrograms::Ptr& programs) { return std::make_shared<GLMaterials>(programs); }

		GLMaterials(const GLPrograms::Ptr& programs) noexcept;

		~GLMaterials() noexcept;

		//����ǰ�˵�material�� ���غ�˶�Ӧ��DriverMaterial
		GLMaterial::Ptr get(const Material::Ptr& material) noexcept;

		void onMaterialDispose(const EventBase::Ptr& event);

		//��������uniform����
		static void refreshMaterialUniforms(UniformHandleMap& uniformHandleMap, const Material::Ptr& material);

		static void refreshMaterialPhong(UniformHandleMap& uniformHandleMap, const MeshPhongMaterial::Ptr& material);

		static void refreshMaterialBasic(UniformHandleMap& uniformHandleMap, const MeshBasicMaterial::Ptr& material);

		static void refreshMaterialCube(UniformHandleMap& uniformHandleMap, const CubeMaterial::Ptr& material);

	private:
		GLPrograms::Ptr mPrograms{ nullptr };

		//key-material id, value-driverMaterial
		std::unordered_map<ID, GLMaterial::Ptr> mMaterials{};
	};
}