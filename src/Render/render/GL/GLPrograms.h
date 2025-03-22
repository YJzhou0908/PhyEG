#pragma once
#include "../../basic/common.h"
#include "../../objects/renderableObject.h"
#include "../../material/material.h"
#include "GLUniforms.h"
#include "GLLights.h"
#include "GLShadowMap.h"
#include "../shaders/uniforms.h"
#include "GLTextures.h"

namespace pe {

	class GLPrograms;
	class GLProgram {
		friend GLPrograms;
	public:
		struct Parameters {
			using Ptr = std::shared_ptr<Parameters>;
			static Ptr create() { return std::make_shared<Parameters>(); }

			std::string		mShaderID;//material ��Typename
			std::string		mVertex;//vs�Ĵ���
			std::string		mFragment;//fs�Ĵ���

			bool			mInstancing{ false };//�Ƿ�����ʵ������,Todo
			bool			mHasNormal{ false };//���λ��Ƶ�ģ���Ƿ��з���
			bool			mHasUV{ false };//���λ��Ƶ�ģ���Ƿ���uv
			bool			mHasColor{ false };//���λ��Ƶ�ģ���Ƿ��ж�����ɫ
			bool			mHasDiffuseMap{ false };//���λ��Ƶ�ģ����ʹ�õĲ����Ƿ���diffuseMap
			bool			mHasEnvCubeMap{ false };//���λ��Ƶ�ģ����ʹ�õĲ����Ƿ��л�����ͼ
			bool			mHasSpecularMap{ false };//���λ��Ƶ�ģ����ʹ�õĲ����Ƿ��о��淴����ͼ

			bool			mShadowMapEnabled{ false };//�Ƿ�������Ӱ
			uint32_t		mDirectionalLightCount{ 0 };
			uint32_t		mNumDirectionalLightShadows{ 0 };

			bool			mUseTangent{ false };
			bool			mUseNormalMap{ false };

			bool			mSkinning{ false };
			uint32_t		mMaxBones{ 0 };

			uint32_t		mDepthPacking{ 0 };
		};

		using Ptr = std::shared_ptr<GLProgram>;
		static Ptr create(const Parameters::Ptr& parameters) {
			return std::make_shared <GLProgram>(parameters);
		}

		GLProgram(const Parameters::Ptr& parameters) noexcept;

		~GLProgram() noexcept;

		auto getID() const noexcept { return mID; }

		auto getCacheKey() const noexcept { return mCacheKey; }

		GLuint		mProgram{ 0 };

		void uploadUniforms(UniformHandleMap& uniformGroup, const GLTextures::Ptr& textures);

	private:
		void replaceAttributeLocations(std::string& shader) noexcept;
		void replaceLightNumbers(std::string& shader, const Parameters::Ptr& parameters) noexcept;

		std::string getExtensionString() noexcept;

	private:
		uint32_t	mID{ 0 };//GLProgram �Լ���id��
		HashType	mCacheKey{ 0 };//��parameters�����ϼ����������hashֵ
		uint32_t	mRefCount{ 0 };//��������ж������ñ�Program��renderItem
		GLUniforms::Ptr mUniforms = nullptr;
	};

	//1 ����GLProgram�Ĺ���,�洢����һ��map��key��program�Ĺ�ϣֵ��value����DriverProgram������ָ��
	//2 ���Ⱪ¶�����߼����еĽӿ�
	class GLPrograms {
	public:
		using Ptr = std::shared_ptr<GLPrograms>;
		static Ptr create() {
			return std::make_shared <GLPrograms>();
		}

		GLPrograms() noexcept;

		~GLPrograms() noexcept;

		GLProgram::Ptr acquireProgram(const GLProgram::Parameters::Ptr& parameters, HashType cacheKey) noexcept;

		UniformHandleMap getUniforms(const Material::Ptr& material) noexcept;

		//���뵱ǰ��Ⱦ����Ĳ��ʡ�object3D����Դ��Ϣ����Ӱ��Ϣ������Щ��������
		//��ȡ����shader����Ҫ����Ϣ�����һ��parameters����
		GLProgram::Parameters::Ptr getParameters(
			const Material::Ptr& material,
			const Object3D::Ptr& object,
			const GLLights::Ptr& lights,
			const GLShadowMap::Ptr& shadowMap) noexcept;

		HashType getProgramCacheKey(const GLProgram::Parameters::Ptr& parameters) noexcept;

		void release(const GLProgram::Ptr& program) noexcept;

	private:
		//key-paramters���ɵĹ�ϣֵ��value-�ñ�parameters���ɵ�driverProgram
		std::unordered_map<HashType, GLProgram::Ptr> mPrograms{};
	};
}