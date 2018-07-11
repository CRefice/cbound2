cmake_minimum_required(VERSION 3.5)
project(Gooey)

option(GOOEY_BUILD_EXAMPLES "Build and install all examples" OFF)

macro(add_gooey_executable target sources)
	add_executable(${target} WIN32 ${sources})
	if(WIN32)
		set_target_properties(${target} PROPERTIES LINK_FLAGS "/ENTRY:mainCRTStartup")
	endif()
endmacro()

add_subdirectory(Source)

if(GOOEY_BUILD_EXAMPLES)
	add_subdirectory(Examples)
endif()

file(GLOB Includes "Include/*.hpp")
install(FILES ${Includes} DESTINATION Include)
