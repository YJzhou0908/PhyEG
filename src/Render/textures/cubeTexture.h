#pragma once 
#include "../basic/common.h"
#include "../basic/constants.h"
#include "texture.h"

namespace pe {

	class CubeTexture:public Texture {
	public:
		static const uint32_t CUBE_TEXTURE_COUNT = 6;
		using Ptr = std::shared_ptr<CubeTexture>;
		static Ptr create(
			const uint32_t& width,
			const uint32_t& height,
			const DataType& dataType = DataType::UnsignedByteType,
			const TextureWrapping& wrapS = TextureWrapping::RepeatWrapping,
			const TextureWrapping& wrapT = TextureWrapping::RepeatWrapping,
			const TextureWrapping& wrapR = TextureWrapping::RepeatWrapping,
			const TextureFilter& magFilter = TextureFilter::NearestFilter,
			const TextureFilter& minFilter = TextureFilter::NearestFilter,
			const TextureFormat& format = TextureFormat::RGBA
		) {
			return std::make_shared<CubeTexture>(
				width,
				height,
				dataType,
				wrapS,
				wrapT,
				wrapR,
				magFilter,
				minFilter,
				format);
		}

		CubeTexture(
			const uint32_t& width,
			const uint32_t& height,
			const DataType& dataType = DataType::UnsignedByteType,
			const TextureWrapping& wrapS = TextureWrapping::RepeatWrapping,
			const TextureWrapping& wrapT = TextureWrapping::RepeatWrapping,
			const TextureWrapping& wrapR = TextureWrapping::RepeatWrapping,
			const TextureFilter& magFilter = TextureFilter::NearestFilter,
			const TextureFilter& minFilter = TextureFilter::NearestFilter,
			const TextureFormat& format = TextureFormat::RGBA
		) noexcept;

		~CubeTexture() noexcept;

		Source::Ptr mSources[CUBE_TEXTURE_COUNT] = { nullptr };
	};
}