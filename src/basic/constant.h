#pragma once 
#include "common.h"

namespace pe {
	enum class BufferAllocType {
		StaticDrawBuffer,
		DynamicDrawBuffer
	};
	enum class DataType {
		UnsignedByteType,
		ByteType,
		Int32Type,
		UInt32Type,
		FloatType
	};

	template<typename T>
	static DataType toDataType() {
		if (typeid(T) == typeid(float)) {
			return DataType::FloatType;
		}

		if (typeid(T) == typeid(int)) {
			return DataType::Int32Type;
		}

		if (typeid(T) == typeid(char)) {
			return DataType::ByteType;
		}

		if (typeid(T) == typeid(unsigned char)) {
			return DataType::UnsignedByteType;
		}

		if (typeid(T) == typeid(uint32_t)) {
			return DataType::UInt32Type;
		}

		return DataType::FloatType;
	}


}