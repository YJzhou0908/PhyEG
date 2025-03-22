#pragma once
#include "../../../basic/common.h"

namespace pe {
	static const std::string colorParseFragment =
		"#ifdef HAS_COLOR\n"\
		"	in vec3 fragColor;\n"\
		"#endif\n"\
		"\n";
}
