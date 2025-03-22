#include "GLAttributes.h"

namespace pe {

	GLAttribute::GLAttribute() noexcept {}

	GLAttribute::~GLAttribute() noexcept {
		if (mHandle) {
			glDeleteBuffers(1, &mHandle);
		}
	}

	GLAttributes::GLAttributes() noexcept {
		EventDispatcher::getInstance()->addEventListener("attributeDispose", this, &GLAttributes::onAttributeDispose);
	}

	GLAttributes::~GLAttributes() noexcept {
		EventDispatcher::getInstance()->removeEventListener("attributeDispose", this, &GLAttributes::onAttributeDispose);
	}

	void GLAttributes::remove(ID attributeID) noexcept {
		auto iter = mAttributes.find(attributeID);
		if (iter != mAttributes.end()) {
			mAttributes.erase(iter);
		}
	}

	void GLAttributes::onAttributeDispose(const EventBase::Ptr& e) {
		ID attrID = *((ID*)e->mpUserData);
		remove(attrID);
	}


}