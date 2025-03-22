#pragma once
#include "../../../basic/common.h"

namespace pe {
	static const std::string uvVertex =
		"#ifdef HAS_UV\n"\
		"	fragUV = uv;\n"\
		"#endif\n"\
		"\n";
}