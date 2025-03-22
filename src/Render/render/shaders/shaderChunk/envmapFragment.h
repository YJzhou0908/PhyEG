#pragma once
#include "../../../basic/common.h"

namespace pe{

	static const std::string envMapFragment =
		"#ifdef USE_ENVMAP\n"\
		"	vec4 envColor = texture(envMap, uvw);\n"\
		"#endif\n"\
		"\n";
}