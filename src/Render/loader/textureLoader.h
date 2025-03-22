#pragma once
#include "loader.h"
#include "../textures/texture.h"

namespace pe {

	class TextureLoader:public Loader {
	public:
		TextureLoader() noexcept;

		~TextureLoader() noexcept;

		// 路径 内存内的数据
		static Texture::Ptr load(const std::string& path, unsigned char* dataIn = nullptr, uint32_t widthIn = 0, uint32_t heightIn = 0);
	};
}