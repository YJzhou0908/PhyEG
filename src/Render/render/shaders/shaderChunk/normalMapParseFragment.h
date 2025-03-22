#pragma once
#include "../../../basic/common.h"

namespace pe {
	static const std::string normalMapParseFragment =
		"#ifdef USE_NORMALMAP\n"\
		"	uniform sampler2D normalMap;\n"\
		"#endif\n"\
		"\n";
}