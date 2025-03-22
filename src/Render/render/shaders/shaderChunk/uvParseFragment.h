#pragma once
#include "../../../basic/common.h"

namespace pe {
	static const std::string uvParseFragment =
		"#ifdef HAS_UV\n"\
		"	in vec2 fragUV;\n"\
		"#endif\n"\
		"\n";
}