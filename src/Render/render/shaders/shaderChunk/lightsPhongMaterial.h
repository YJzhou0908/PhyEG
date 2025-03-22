#pragma once
#include "../../../basic/common.h"

namespace pe {
	static const std::string lightsPhongMaterial =
		"BlinnPhongMaterial material;\n"\
		"material.diffuseColor = diffuseColor.rgb;\n"\
		"material.specularShininess = shininess;\n"\
		"material.specularStrength = specularStrength;\n"\
		"\n";
}