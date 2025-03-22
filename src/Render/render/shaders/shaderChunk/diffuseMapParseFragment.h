#pragma once
#include "../../../basic/common.h"
namespace pe {

	static const std::string diffuseMapParseFragment =
		"#ifdef HAS_DIFFUSE_MAP\n"\
		"	uniform sampler2D diffuseMap;\n"\
		"#endif\n"\
		"\n";
}