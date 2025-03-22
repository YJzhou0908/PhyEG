#pragma once
#include "../../../basic/common.h"

namespace pe {
	static const std::string normalFragmentMap =
		"#ifdef USE_NORMALMAP\n"\
		"	normal = texture2D(normalMap, fragUV).xyz * 2.0 - 1.0;\n"\
		"	normal = normalize(TBN * normal);\n"\
		"#endif\n"\
		"\n";
}