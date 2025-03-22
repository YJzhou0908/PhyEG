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

		//ͨ��glGenTextures��õ�texture�ı��
		GLuint	mHandle{ 0 };

	};


	class GLTextures {
	public:
		using Ptr = std::shared_ptr<GLTextures>;

		// һ��Texture��������һ��RenderTargets�ϲſ���
		static Ptr create(const GLInfo::Ptr& info, const GLRenderTargets::Ptr& renderTargets) {
			return std::make_shared <GLTextures>(info, renderTargets);
		}

		GLTextures(const GLInfo::Ptr& info, const GLRenderTargets::Ptr& renderTargets) noexcept;

		~GLTextures() noexcept;

		//����texture��������Ӧ��DriverTexture
		GLTexture::Ptr get(const Texture::Ptr& texture) noexcept;

		//����:
		// ��texture��Ӧ��Driver Texture���е�mHandle���󶨵�textureUnit��slot��
		//GL_TEXTURE0 GL_TEXTURE1....
		void bindTexture(const Texture::Ptr& texture, GLenum textureUnit);

		void setupRenderTarget(const RenderTarget::Ptr& renderTarget) noexcept;

		void onTextureDestroy(const EventBase::Ptr& e) noexcept;

	private:
		//Ҫô�½�һ��texture �� Ҫô����ԭ��texture���������ݻ�����������
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