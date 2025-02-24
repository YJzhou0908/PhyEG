#pragma once 
#include "../basic/common.h"

namespace pe {
	class Identity {
	public:
		static ID generateID() {
			return IDPos++;
		};
	private:
		static ID IDPos{0};


	};

}