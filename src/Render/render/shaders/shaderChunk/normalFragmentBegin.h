#pragma once
#include "../../../basic/common.h"

namespace pe {
	static const std::string normalFragmentBegin =
		"#ifdef HAS_NORMAL\n"\
		"	vec3 normal = normalize(fragNormal);\n"\
		"	#ifdef USE_TANGENT\n"\
		"		vec3 tangent = normalize(fragTangent);\n"\
		"		vec3 bitangent = normalize(fragBitangent);\n"\
		"		mat3 TBN = mat3(tangent, bitangent, normal);\n"\
		"	#endif\n"\
		"#endif\n"\
		"\n";
}