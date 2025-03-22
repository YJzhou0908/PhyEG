#pragma once
#include "../../../basic/common.h"

namespace pe {
	static const std::string worldPositionVertex =
		"#if defined(USE_SHADOWMAP) || defined(USE_ENVMAP)\n"\
		"	vec4 worldPosition = vec4(transformed, 1.0);\n"\
		"	worldPosition = modelMatrix * worldPosition;\n"\
		"#endif\n"\
		"\n";
}