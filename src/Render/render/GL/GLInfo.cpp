#include "GLInfo.h"

namespace pe {

	GLInfo::GLInfo() noexcept {}

	GLInfo::~GLInfo() noexcept {}

	void GLInfo::reset() noexcept {
		mRender.mFrame++;
		mRender.mCalls = 0;
		mRender.mTriangels = 0;
	}

}