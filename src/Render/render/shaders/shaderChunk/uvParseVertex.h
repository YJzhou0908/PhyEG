#pragma once
#include "../../../basic/common.h"

namespace pe {
	static const std::string uvParseVertex =
		"#ifdef HAS_UV\n"\
		"	layout(location = UV_LOCATION) in vec2 uv;\n"\
		"	out vec2 fragUV;\n"\
		"#endif\n"\
		"\n";
}