#include "GLTextures.h"
#include "../MultipleRenderTarget.h"

namespace pe {

	GLTexture::GLTexture() noexcept {
	}

	GLTexture::~GLTexture() noexcept {
		dispose();
	}

	void GLTexture::dispose() noexcept {
		if (mHandle) {
			glDeleteTextures(GL_TEXTURE_2D, &mHandle);
			mHandle = 0;
		}
	}

	GLTextures::GLTextures(const GLInfo::Ptr& info, const GLRenderTargets::Ptr& renderTargets) noexcept {
		mInfo = info;
		mRenderTargets = renderTargets;

		EventDispatcher::getInstance()->addEventListener("textureDispose", this, &GLTextures::onTextureDestroy);
	}

	GLTextures::~GLTextures() noexcept {
		EventDispatcher::getInstance()->removeEventListener("textureDispose", this, &GLTextures::onTextureDestroy);
	}

	void GLTextures::update(const Texture::Ptr& texture) noexcept {
		auto dTexture = get(texture);

		//mNeedsUpdate��texture���δ�����ʱ�򣬻���true
		//��ʹ���߸�����texture��صĶ���֮�󣬿����ֶ�������Ϊtrue���ͻᴥ������
		if (texture->mNeedsUpdate) {
			texture->mNeedsUpdate = false;
			setupDriverTexture(texture);
		}

	}

	GLTexture::Ptr GLTextures::setupDriverTexture(const Texture::Ptr& texture) noexcept {
		GLTexture::Ptr dtexture = get(texture);
		texture->mNeedsUpdate = false;

		if (!dtexture->mHandle) {
			glGenTextures(1, &dtexture->mHandle);
		}
		glBindTexture(toGL(texture->mTextureType), dtexture->mHandle);

		//�����������
		glTexParameteri(toGL(texture->mTextureType), GL_TEXTURE_MIN_FILTER, toGL(texture->mMinFilter));
		glTexParameteri(toGL(texture->mTextureType), GL_TEXTURE_MAG_FILTER, toGL(texture->mMagFilter));
		glTexParameteri(toGL(texture->mTextureType), GL_TEXTURE_WRAP_S, toGL(texture->mWrapS));
		glTexParameteri(toGL(texture->mTextureType), GL_TEXTURE_WRAP_T, toGL(texture->mWrapT));
		glTexParameteri(toGL(texture->mTextureType), GL_TEXTURE_WRAP_R, toGL(texture->mWrapR));

		if (texture->mTextureType == TextureType::Texture2D) {
			//��������ͼר�õ�texture��������ȾĿ�꣬�ſ�����ͼƬ����
			const byte* data = (texture->getUsage() == TextureUsage::SamplerTexture) ? texture->mSource->mData.data() : nullptr;

			//1 �����ڴ�ռ�
			//2 ����ͼƬ����
			glTexImage2D(GL_TEXTURE_2D, 0, toGL(texture->mInternalFormat), texture->mWidth, texture->mHeight, 0, toGL(texture->mFormat), toGL(texture->mDataType), data);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else {

			//Ϊ��ǰ��cubeMap��texture�������ڴ濪���Լ����ݸ���
			for (uint32_t i = 0; i < CubeTexture::CUBE_TEXTURE_COUNT; ++i) {
				auto cubeTexture = std::static_pointer_cast<CubeTexture>(texture);
				const byte* data = (texture->getUsage() == TextureUsage::SamplerTexture) ? cubeTexture->mSources[i]->mData.data() : nullptr;

				//�����ڴ漰�������ݵ�˳����������ǰ��
				//Ҫ����һ���濪���ڴ�������ݣ���������һ�����target
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, toGL(texture->mInternalFormat), texture->mWidth, texture->mHeight, 0, toGL(texture->mFormat), toGL(texture->mDataType), data);
			}
		}

		glBindTexture(toGL(texture->mTextureType), 0);
		mInfo->mMemory.mTextures++;

		return dtexture;
	}

	void GLTextures::setupFBOColorAttachment(const GLuint& fbo, const GLenum& attachment, const Texture::Ptr& texture) noexcept {
		auto dTexture = get(texture);

		glBindFramebuffer(GL_FRAMEBUFFER, fbo);
		glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, toGL(texture->mTextureType), dTexture->mHandle, 0);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void GLTextures::setupFBODepthStencilAttachment(const RenderTarget::Ptr& renderTarget) noexcept {
		auto dRenderTarget = mRenderTargets->get(renderTarget);
		if (!dRenderTarget->mFrameBuffer) {
			std::cout << "Error: frameBuffer has not been generated,when setupFBODepthStencilAttachment!" << std::endl;
			return;
		}

		//TODO: support stencil buffer
		//�����ҪDepthTest��������Ҫָ����һ���û�������Texture
		if (renderTarget->mNeedsDepth && renderTarget->mDepthTexture) {
			setupDepthTexture(dRenderTarget->mFrameBuffer, renderTarget);
		}
		//���û��ָ�����Լ�Ĭ�ϴ���һ��������ȼ���RenderBuffer
		else if (renderTarget->mNeedsDepth) {
			setupDepthRenderBuffer(dRenderTarget->mFrameBuffer, renderTarget);
		}

	}

	void GLTextures::setupDepthTexture(const GLuint& frameBuffer, const RenderTarget::Ptr& renderTarget) noexcept {
		if (!renderTarget->mDepthTexture || !renderTarget->mNeedsDepth) {
			std::cout << "Error: renderTarget has no depth info when setupDepthTexture";
			return;
		}
		auto depthTexture = renderTarget->mDepthTexture;
		auto dDepthTexture = get(depthTexture);
		setupDriverTexture(depthTexture);

		glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, toGL(depthTexture->mTextureType), dDepthTexture->mHandle, 0);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void GLTextures::setupDepthRenderBuffer(const GLuint& frameBuffer, const RenderTarget::Ptr& renderTarget) {
		auto dRenderTarget = mRenderTargets->get(renderTarget);

		//����RenderBuffer
		glGenRenderbuffers(1, &dRenderTarget->mDepthRenderBuffer);
		glBindRenderbuffer(GL_RENDERBUFFER, dRenderTarget->mDepthRenderBuffer);

		//ΪrenderBuffer���󿪱ٿռ�
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, renderTarget->mWidth, renderTarget->mHeight);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);


		//��frameBuffer���а�
		glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, dRenderTarget->mDepthRenderBuffer);

		GLint error = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	GLTexture::Ptr GLTextures::get(const Texture::Ptr& texture) noexcept {
		auto iter = mTextures.find(texture->getID());
		if (iter == mTextures.end()) {
			iter = mTextures.insert(std::make_pair(texture->getID(), GLTexture::create())).first;
		}

		return iter->second;
	}

	void GLTextures::bindTexture(const Texture::Ptr& texture, GLenum textureUnit) {
		//GL_TEXTURE0  GL_TEXTURE1 GL_TEXTURE2....
		//GL_TEXTURE1 = GL_TEXTURE0+1
		//GL_TEXTURE2 = GL_TEXTURE0+2
		glActiveTexture(textureUnit);
		//���»��ߴ���textureID
		update(texture);
		auto dTexture = get(texture);
		glBindTexture(toGL(texture->mTextureType), dTexture->mHandle);
	}

	void GLTextures::setupRenderTarget(const RenderTarget::Ptr& renderTarget) noexcept {
		auto dRenderTarget = mRenderTargets->get(renderTarget);

		//generate framebuffer
		if (!dRenderTarget->mFrameBuffer) {
			dRenderTarget->generateFrameBuffer();
		}

		//setup color attachments
		if (renderTarget->mIsMultiRenderTarget) {
			auto multiRT = std::static_pointer_cast<MultipleRenderTarget>(renderTarget);
			auto textures = multiRT->mTextures;

			for (uint32_t i = 0; i < textures.size(); ++i) {
				auto texture = textures[i];
				setupDriverTexture(texture);
				setupFBOColorAttachment(dRenderTarget->mFrameBuffer, GL_COLOR_ATTACHMENT0 + i, texture);
			}
		}
		else {
			auto texture = renderTarget->mTexture;
			setupDriverTexture(texture);
			setupFBOColorAttachment(dRenderTarget->mFrameBuffer, GL_COLOR_ATTACHMENT0, texture);
		}

		//setup depth and stencil
		if (renderTarget->mNeedsDepth) {
			setupFBODepthStencilAttachment(renderTarget);
		}
	}

	void GLTextures::onTextureDestroy(const EventBase::Ptr& e) noexcept {
		auto texture = static_cast<Texture*>(e->mTarget);
		auto id = texture->getID();

		auto iter = mTextures.find(texture->getID());
		if (iter != mTextures.end()) {
			mTextures.erase(iter);
			mInfo->mMemory.mTextures--;
		}
	}
}