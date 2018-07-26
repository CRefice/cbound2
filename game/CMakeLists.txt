set(SRCS
	source/main.cpp
)

set(LIBS
	cbound-core
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

add_executable(cbound-game "${SRCS}")
target_link_libraries(cbound-game "${LIBS}")
target_compile_options(cbound-game PRIVATE "${OPTS}")
target_compile_features(cbound-game PRIVATE cxx_std_17)

# Add a custom copy-resources command
set(RESOURCE_INPUT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/resources")
set(RESOURCE_OUTPUT_DIR "${CMAKE_CURRENT_BINARY_DIR}/resources")

add_custom_command(
	TARGET cbound-game POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E create_symlink
	${RESOURCE_INPUT_DIR}
	${RESOURCE_OUTPUT_DIR}
	)
