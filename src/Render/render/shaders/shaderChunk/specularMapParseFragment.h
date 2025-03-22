#pragma once
#include "../../../basic/common.h"

namespace pe {

	static const std::string specularMapParseFragment =
		"#ifdef USE_SPECULARMAP\n"\
		"	uniform sampler2D specularMap;\n"\
		"#endif\n"\
		"\n";
}