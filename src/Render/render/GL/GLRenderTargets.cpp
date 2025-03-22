#include "GLRenderTargets.h"
#include "../../basic/eventDispatcher.h"

namespace pe {
	GLRenderTarget::GLRenderTarget() noexcept {}

	GLRenderTarget::~GLRenderTarget() noexcept {
		if (mFrameBuffer) {
			glDeleteFramebuffers(1, &mFrameBuffer);
		}

		if (mDepthRenderBuffer) {
			glDeleteRenderbuffers(1, &mDepthRenderBuffer);
		}
	}

	void GLRenderTarget::generateFrameBuffer() noexcept {
		glGenFramebuffers(1, &mFrameBuffer);
	}

	GLRenderTargets::GLRenderTargets() noexcept {
		EventDispatcher::getInstance()->addEventListener("renderTargetDispose", this, &GLRenderTargets::onRenderTargetDispose);
	}

	GLRenderTargets::~GLRenderTargets() noexcept {
		EventDispatcher::getInstance()->removeEventListener("renderTargetDispose", this, &GLRenderTargets::onRenderTargetDispose);
	}

	GLRenderTarget::Ptr GLRenderTargets::get(const RenderTarget::Ptr& renderTarget) noexcept {
		auto id = renderTarget->mID;

		auto iter = mRenderTargets.find(id);
		if (iter == mRenderTargets.end()) {
			iter = mRenderTargets.insert(std::make_pair(id, GLRenderTarget::create())).first;
		}

		return iter->second;
	}

	void GLRenderTargets::onRenderTargetDispose(const EventBase::Ptr& e) {
		auto renderTarget = static_cast<RenderTarget*>(e->mTarget);
		auto iter = mRenderTargets.find(renderTarget->mID);
		if (iter == mRenderTargets.end()) {
			return;
		}

		mRenderTargets.erase(iter);
	}

}
