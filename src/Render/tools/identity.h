#pragma once 
#include "../basic/common.h"

namespace pe {
	class Identity {
	public:
		static ID generateID() {
			return mCurrentID++;
		};
	private:
		static ID mCurrentID;


	};

}