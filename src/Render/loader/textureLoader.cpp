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

		//���·��Ϊ�գ���ʹ��Ĭ��ͼƬ
		if (filePath.empty()) {
			filePath = DefaultTexturePath;
		}

		//����Ƿ��Ѿ����ɹ�source����������˾ʹ�cache����ȡ����
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

			//������ȡ������ͼƬ���ݴ�С
			uint32_t dataSize{ 0 };

			//��ȡ������ͼƬ����ָ��
			unsigned char* bits{ nullptr };

			//Ҫô��Ӳ�̶�ȡ��Ҫô����������ȡ
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
				//��¼���������ݵĴ�С
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

			//�����������̣�����׼���������еı�Ҫ���ݣ����������source��data(Vector<Byte>)
			if (dataSize && bits) {
				data.resize(dataSize);

				//��bits��data�ĵ�ַ��ͷ������dataSize��Byte������
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