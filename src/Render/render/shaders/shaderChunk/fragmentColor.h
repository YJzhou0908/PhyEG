#pragma once
#include "../../../basic/common.h"

namespace pe {

	static const std::string FragmentCommonEnding =
		"#ifdef HAS_COLOR\n"\
		"	fragmentColor = vec4(color, 1.0);\n"\
		"#else\n"\
		"	fragmentColor = vec4(1.0, 1.0, 1.0, 1.0);\n"\
		"#endif\n";

}