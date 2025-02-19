#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <unordered_map>
#include <functional>
#include <tuple>
#include <bitset>
#include <algorithm>
#include <deque>
#include <regex>
#include <random>
#include <any>
#include <chrono>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/matrix_decompose.hpp"


using ID = uint32_t;

struct Range {
	int32_t mOffset{ 0 };
	int32_t mCount{ -1 };
};

