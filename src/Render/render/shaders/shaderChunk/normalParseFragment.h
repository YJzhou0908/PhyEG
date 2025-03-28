#pragma once
#include "../../../basic/common.h"

namespace pe {
	static const std::string normalParseFragment =
		"#ifdef HAS_NORMAL\n"\
		"	in vec3 fragNormal;\n"\
		"	#ifdef USE_TANGENT\n"\
		"		in vec3 fragTangent;\n"\
		"		in vec3 fragBitangent;\n"\
		"	#endif\n"\
		"#endif\n"\
		"\n";
}