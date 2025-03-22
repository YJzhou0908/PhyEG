file(GLOB_RECURSE PE ./  *.cpp  *.c)

include_directories(
	SYSTEM ${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/include ${CMAKE_CURRENT_SOURCE_DIR}/src
)

add_library(PE_lib  ${PE})