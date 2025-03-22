#pragma once 
#include "../../../basic/common.h"
#include "../shaderChunk/shaderChunk.h"

namespace pe {

	namespace cube {
		static const std::string vertex =
			common +
			positionParseVertex +
			"out vec3 uvw;\n"+
			uniformMatricesVertex +

			"void main() {\n" +
				"uvw = position;\n"+
				beginVertex +
				projectVertex +
				"gl_Position.z = gl_Position.w;\n"+ // 确保深度都是1，就是最深的那个，这样的话就不会把内部渲染的物体遮挡住。
			"}\n";

		static const std::string fragment =
			envMapCommonParseFragment +
			"in vec3 uvw; \n"
			"out vec4 fragmentColor;\n" +

			"void main() {\n" +
				envMapFragment + 
			"	fragmentColor = envColor;\n" +
			"}";
	}
}