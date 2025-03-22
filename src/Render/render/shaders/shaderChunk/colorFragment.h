#pragma once
#include "../../../basic/common.h"

namespace pe {
	static const std::string colorFragment =
		"#ifdef HAS_COLOR\n"\
		"	diffuseColor.rgb *= fragColor;\n"\
		"#endif\n"\
		"\n";
}