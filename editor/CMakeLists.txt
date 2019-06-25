set(SRCS
	source/tiles.cpp
	source/io.cpp
)

set(LIBS
	cbound-core
	imgui
	stdc++fs
	${OPENGL_gl_LIBRARY}
)

if(MSVC)
  # Force to always compile with W4
	string(APPEND OPTS "/W4")
else()
  # Update if necessary
	string(APPEND OPTS "-Wall;-pedantic")
endif()

add_library(cbound-editor ${SRCS})
target_include_directories(cbound-editor PUBLIC include/)
target_include_directories(cbound-editor PRIVATE include/editor/)
target_link_libraries(cbound-editor PUBLIC ${LIBS})
target_compile_options(cbound-editor PUBLIC ${OPTS})
target_compile_features(cbound-editor PUBLIC cxx_std_17)
if (${CMAKE_BUILD_TYPE} MATCHES "DEBUG")
	target_compile_definitions(cbound-editor PRIVATE "SOL_CHECK_ARGUMENTS" "SOL_PRINT_ERRORS")
endif()
