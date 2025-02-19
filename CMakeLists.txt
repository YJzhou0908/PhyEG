cmake_minimum_required(VERSION 3.10)
project(PhysicalEngine)

set(CMAKE_CXX_STANDARD 17)

file(GLOB copylist "./assets" )
file(COPY ${copylist} DESTINATION ${CMAKE_BINARY_DIR})

include_directories(
SYSTEM ${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/include
)

link_directories(
	SYSTEM ${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/libs/glfw
	SYSTEM ${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/libs/assimp
)


# add_subdirectory(src)

# add_executable(triangle "examples/triangle.cpp")
 
# target_link_libraries