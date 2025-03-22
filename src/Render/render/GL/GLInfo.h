#pragma once
#include "../../basic/common.h"

namespace pe {

	//������Ⱦ������Ⱦ�����еĸ���ָ�����ݼ�¼
	class GLInfo {
	public:
		struct Memory {
			uint32_t mGeometries{ 0 };
			uint32_t mTextures{ 0 };
		};

		struct Render {
			uint32_t	mFrame{ 0 };
			uint32_t	mCalls{ 0 };
			uint32_t	mTriangels{ 0 };
		};

		using Ptr = std::shared_ptr<GLInfo>;
		static Ptr create() {
			return std::make_shared <GLInfo>();
		}

		GLInfo() noexcept;

		~GLInfo() noexcept;

		void reset() noexcept;

	public:
		Memory	mMemory{};
		Render	mRender{};
	};

}