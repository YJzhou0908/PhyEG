cmake_minimum_required(VERSION 3.10)
project(PhysicalEngine)

set(CMAKE_CXX_STANDARD 17)

file(GLOB copylist "./assets" )
file(COPY ${copylist} DESTINATION ${CMAKE_BINARY_DIR})

include_directories(
	SYSTEM ${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/include ${CMAKE_CURRENT_SOURCE_DIR}/src
)

link_directories(
	SYSTEM ${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/libs/glfw ${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/libs/assimp
)


add_subdirectory(src)

add_executable(triangle "./examples/triangle.cpp") 
add_executable(material "./examples/materials.cpp")
add_executable(skybox "./examples/skybox.cpp")
add_executable(shadow "./examples/shadow.cpp")
add_executable(offscreen "./examples/offscreen.cpp")

target_link_libraries(triangle PE_lib glfw3.lib assimp-vc143-mt.lib )
target_link_libraries(material PE_lib glfw3.lib assimp-vc143-mt.lib )
target_link_libraries(skybox PE_lib glfw3.lib assimp-vc143-mt.lib )
target_link_libraries(shadow PE_lib glfw3.lib assimp-vc143-mt.lib )
target_link_libraries(offscreen PE_lib glfw3.lib assimp-vc143-mt.lib )