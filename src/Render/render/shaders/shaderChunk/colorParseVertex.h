#pragma once
#include "../../../basic/common.h"

namespace pe {
	static const std::string colorParseVertex =
		"#ifdef HAS_COLOR\n"\
		"	layout(location = COLOR_LOCATION) in vec3 color;\n"\
		"	out vec3 fragColor;\n"\
		"#endif\n"\
		"\n";
}