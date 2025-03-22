#pragma once
#include "../../../basic/common.h"

namespace pe {

	static const std::string specularMapFragment =
		"float specularStrength = 1.0;\n"\
		"#ifdef USE_SPECULARMAP\n"\
		"	specularStrength = texture2D(specularMap, fragUV).r;\n"\
		"#endif\n"\
		"\n";
}