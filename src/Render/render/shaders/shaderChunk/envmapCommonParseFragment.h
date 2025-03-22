#pragma once
#include "../../../basic/common.h"

namespace pe {

	static const std::string envMapCommonParseFragment =
		"#ifdef USE_ENVMAP\n"\
		"	uniform samplerCube envMap;\n"\
		"#endif\n"\
		"\n";
}