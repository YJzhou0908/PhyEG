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

	using KeyboardAction = uint64_t;

	static constexpr KeyboardAction WKey = 1;
	static constexpr KeyboardAction SKey = 2;
	static constexpr KeyboardAction AKey = 3;
	static constexpr KeyboardAction DKey = 4;
	static constexpr KeyboardAction KeyNONE = 31;

#define DOWN	0
#define UP		1

	//key :代表了某一个按键
	//value: 对应的Action编号
	using KeyboardMapType = std::unordered_map<uint32_t, KeyboardAction>;

	static const KeyboardMapType KeyboardActionMap = {
		KeyboardMapType::value_type(GLFW_KEY_W, WKey),
		KeyboardMapType::value_type(GLFW_KEY_S, SKey),
		KeyboardMapType::value_type(GLFW_KEY_A, AKey),
		KeyboardMapType::value_type(GLFW_KEY_D, DKey),
	};


	enum class MouseAction :uint8_t {
		LeftDown,
		RightDown,
		MiddleDown,
		LeftUp,
		RightUp,
		MiddleUp,
		NONE,
	};

	using MouseActionMapType = std::unordered_map<uint32_t, std::tuple<MouseAction, MouseAction>>;

	static const MouseActionMapType MouseActionMap = {
		MouseActionMapType::value_type(GLFW_MOUSE_BUTTON_LEFT, {MouseAction::LeftDown, MouseAction::LeftUp}),
		MouseActionMapType::value_type(GLFW_MOUSE_BUTTON_RIGHT, {MouseAction::RightDown, MouseAction::RightUp}),
		MouseActionMapType::value_type(GLFW_MOUSE_BUTTON_MIDDLE, {MouseAction::MiddleDown, MouseAction::MiddleUp}),
	};
}