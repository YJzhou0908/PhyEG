#pragma once
#include "../../../basic/common.h"

namespace pe {
	static const std::string colorVertex =
		"#ifdef HAS_COLOR\n"\
		"	fragColor = color;\n"\
		"#endif\n"\
		"\n";
}