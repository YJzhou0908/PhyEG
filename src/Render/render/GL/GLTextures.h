#pragma once
#include "../../basic/common.h"
#include "../../basic/eventDispatcher.h"
#include "../../textures/texture.h"
#include "../../textures/cubeTexture.h"

#include "../renderTarget.h"
#include "GLInfo.h"
#include "GLRenderTargets.h"


namespace pe {
	class GLTexture {
	public:
		using Ptr = std::shared_ptr<GLTexture>;
		static Ptr create() {
			return std::make_shared <GLTexture>();
		}

		GLTexture() noexcept;

		~GLTexture() noexcept;

		void dispose() noexcept;

		//通过glGenTextures获得的texture的编号
		GLuint	mHandle{ 0 };

	};


	class GLTextures {
	public:
		using Ptr = std::shared_ptr<GLTextures>;

		// 一个Texture必须贴在一个RenderTargets上才可以
		static Ptr create(const GLInfo::Ptr& info, const GLRenderTargets::Ptr& renderTargets) {
			return std::make_shared <GLTextures>(info, renderTargets);
		}

		GLTextures(const GLInfo::Ptr& info, const GLRenderTargets::Ptr& renderTargets) noexcept;

		~GLTextures() noexcept;

		//传入texture，获得其对应的DriverTexture
		GLTexture::Ptr get(const Texture::Ptr& texture) noexcept;

		//作用:
		// 将texture对应的Driver Texture当中的mHandle，绑定到textureUnit的slot上
		//GL_TEXTURE0 GL_TEXTURE1....
		void bindTexture(const Texture::Ptr& texture, GLenum textureUnit);

		void setupRenderTarget(const RenderTarget::Ptr& renderTarget) noexcept;

		void onTextureDestroy(const EventBase::Ptr& e) noexcept;

	private:
		//要么新建一个texture ， 要么跟新原有texture的属性数据或者内容数据
		void update(const Texture::Ptr& texture) noexcept;

		GLTexture::Ptr setupDriverTexture(const Texture::Ptr& texture) noexcept;

		void setupFBOColorAttachment(const GLuint& fbo, const GLenum& target, const Texture::Ptr& texture) noexcept;

		void setupFBODepthStencilAttachment(const RenderTarget::Ptr& renderTarget) noexcept;

		void setupDepthTexture(const GLuint& frameBuffer, const RenderTarget::Ptr& renderTarget) noexcept;

		void setupDepthRenderBuffer(const GLuint& frameBuffer, const RenderTarget::Ptr& renderTarget);

	private:
		GLInfo::Ptr								mInfo{ nullptr };
		GLRenderTargets::Ptr					mRenderTargets{ nullptr };
		std::unordered_map<ID, GLTexture::Ptr>	mTextures{};
	};
}