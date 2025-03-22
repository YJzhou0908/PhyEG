#pragma once
#include "../basic/common.h"
#include "../basic/constants.h"
#include "../textures/texture.h"
#include "../textures/depthTexture.h"

namespace pe {

	class RTTypeChecker {
	public:
		bool mIsRenderTarget = false;
		bool mIsMultiRenderTarget = false;
	};

	class GLTextures;
	class GLRenderTargets;
	class RenderTarget :public RTTypeChecker {
	public:
		friend GLTextures;
		friend GLRenderTargets;

		struct Options {
			TextureWrapping	mWrapS{ TextureWrapping::RepeatWrapping };
			TextureWrapping	mWrapT{ TextureWrapping::RepeatWrapping };
			TextureWrapping	mWrapR{ TextureWrapping::RepeatWrapping };
			TextureFilter	mMagFilter{ TextureFilter::LinearFilter };
			TextureFilter	mMinFilter{ TextureFilter::LinearFilter };
			TextureFormat	mFormat{ TextureFormat::RGBA };
			DataType		mDataType{ DataType::UnsignedByteType };
			TextureFormat	mInternalFormat{ TextureFormat::RGBA };

			bool			mNeedsDepthBuffer{ true };
			bool			mNeedsStencilBuffer{ false };

			//我们允许用户给到我们一个他自己生成的Texture作为深度Buffer
			DepthTexture::Ptr	mDepthTexture{ nullptr };
		};

		using Ptr = std::shared_ptr<RenderTarget>;
		static Ptr create(const uint32_t& width, const uint32_t& height, const Options& options) {
			return std::make_shared <RenderTarget>(width, height, options);
		}

		RenderTarget(const uint32_t& width, const uint32_t& height, const Options& options) noexcept;

		~RenderTarget() noexcept;

		void setTexture(const Texture::Ptr& texture) noexcept;

		auto getTexture() const noexcept { return mTexture; }

		virtual void setSize(const uint32_t& width, const uint32_t& height) noexcept;

		void dispose() noexcept;

	public:
		bool	mNeedsUpdate{ true };

	protected:
		ID						mID{ 0 };
		uint32_t				mWidth{ 0 };
		uint32_t				mHeight{ 0 };

		Texture::Ptr			mTexture{ nullptr };//作为ColorAttachment的纹理图片

		bool					mNeedsDepth{ true };
		bool					mNeedsStencil{ false };

		Texture::Ptr			mDepthTexture{ nullptr };//作为depthAttachment的纹理图片
	};
}