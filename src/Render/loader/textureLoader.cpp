#include "textureLoader.h"
#include <stb_image.h>
#include "../basic/config.h"
#include "cache.h"

namespace pe {

	TextureLoader::TextureLoader() noexcept {}

	TextureLoader::~TextureLoader() noexcept {}

	Texture::Ptr TextureLoader::load(const std::string& path, unsigned char* dataIn, uint32_t widthIn, uint32_t heightIn) {
		Texture::Ptr texture = nullptr;
		std::string filePath = path;

		//如果路径为空，则使用默认图片
		if (filePath.empty()) {
			filePath = DefaultTexturePath;
		}

		//检查是否已经生成过source，如果生成了就从cache里面取出来
		Source::Ptr source = Cache::getInstance()->getSource(path);

		if (source) {
			texture = Texture::create(source->mWidth, source->mHeight);
			texture->mSource = source;
		}
		else {
			source = Source::create();
			source->mNeedsUpdate = false;

			auto& data = source->mData;

			int			picType = 0;
			int width = 0, height = 0;

			//整个读取出来的图片数据大小
			uint32_t dataSize{ 0 };

			//读取出来的图片数据指针
			unsigned char* bits{ nullptr };

			//要么从硬盘读取，要么从数据流读取
			if (dataIn == nullptr) {
				//if nofile, use default
				std::fstream file(filePath);
				if (!file.is_open()) {
					filePath = DefaultTexturePath;
				}
				else {
					file.close();
				}

				bits = stbi_load(filePath.c_str(), &width, &height, &picType, toStbImageFormat(TextureFormat::RGBA));
			}
			else {
				//记录了整个数据的大小
				uint32_t dataInSize = 0;

				if (!heightIn) {
					dataInSize = widthIn;
				}
				else {
					dataInSize = widthIn * heightIn;
				}

			
				bits = stbi_load_from_memory(dataIn, dataInSize, &width, &height, &picType, toStbImageFormat(TextureFormat::RGBA));
			}

			dataSize = width * height * toByteSize(TextureFormat::RGBA);

			//经过上述过程，终于准备好了所有的必要数据，接下来填充source的data(Vector<Byte>)
			if (dataSize && bits) {
				data.resize(dataSize);

				//从bits向data的地址开头，拷贝dataSize个Byte的数据
				memcpy(data.data(), bits, dataSize);
			}

			stbi_image_free(bits);

			source->mWidth = width;
			source->mHeight = height;
			texture = Texture::create(source->mWidth, source->mHeight);
		}


		texture->mSource = source;
		Cache::getInstance()->cacheSource(filePath, source);

		return texture;
	}
}