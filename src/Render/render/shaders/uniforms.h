#pragma once 
#include "../../basic/common.h"
#include "../../basic/constants.h"
#include "../../textures/texture.h"
#include "../../textures/cubeTexture.h"

namespace pe {



	class UniformHandle {
	public:
		UniformHandle() noexcept {}

		~UniformHandle() noexcept {}

		std::any	mValue;//�������κ����͵�ֵ������float matrix map
		bool		mNeedsUpdate{ false };
	};

	using UniformUnitMap = std::unordered_map<std::string, std::any>;

	using UniformHandleMap = std::unordered_map<std::string, UniformHandle>;

	//��ShaderChunk�����UniformChunk
	static const std::unordered_map<std::string, UniformHandleMap> UniformsLib =
	{
		{
			"common", {
				{"diffuseMap", UniformHandle()},
				{"opacity", UniformHandle()}
			}
		},
		{
			"envMap", {
				{"envMap", UniformHandle()}
			}
		},
		{
			"specularMap", {
				{"specularMap", UniformHandle()}
			}
		},
		{
			"normalMap", {
				{"normalMap", UniformHandle()}
			}
		}

	};

	//------------tools-------------------------------
	static UniformHandleMap merge(const std::vector<UniformHandleMap>& uniformGroups) {
		UniformHandleMap mergedGroup;

		for (const auto& group : uniformGroups) {
			for (const auto& uniformIter : group) {
				auto name = uniformIter.first;
				auto handle = uniformIter.second;

				mergedGroup.insert(std::make_pair(name, handle));
			}
		}

		return std::move(mergedGroup);
	}

	static UniformUnitMap* setArrayStructuredUniform(const uint32_t& index, UniformUnitMap* array) noexcept {
		auto indexString = std::to_string(index);
		(*array)[indexString] = UniformUnitMap();

		//���ո����ɵ�UniformUnitMap����ת����UniformUnitMap*��cast��ָ������&��������
		return std::any_cast<UniformUnitMap>(&(*array)[indexString]);
	}

	//��array���map���У�ȡ��keyΪindex��uniformUnitMap��ָ��
	static UniformUnitMap* getArrayStructuredUniform(const uint32_t& index, UniformUnitMap* array) noexcept {
		auto indexString = std::to_string(index);
		return std::any_cast<UniformUnitMap>(&(*array)[indexString]);
	}

	//���ߺ�������array�����������
	template<typename T>
	static void clearPureArrayUniform(std::vector<T>* array) {
		array->clear();
	}

	//���ߺ�������value push��array
	template<typename T>
	static void pushPureArrayUniform(T value, std::vector<T>* array) {
		array->push_back(value);
	}

}
