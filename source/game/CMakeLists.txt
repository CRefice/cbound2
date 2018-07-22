set(SRCS
	main.cpp
)

set(LIBS
	cbound-core
	stdc++fs
	${OPENGL_gl_LIBRARY}
)

set(CMAKE_CXX_STANDARD 17)

add_executable(cbound-game "${SRCS}")
target_link_libraries(cbound-game "${LIBS}")

# Add a custom copy-resources command
set(RESOURCE_INPUT_DIR "${PROJECT_SOURCE_DIR}/resources")
set(RESOURCE_OUTPUT_DIR "${CMAKE_CURRENT_BINARY_DIR}/resources")

add_custom_command(
	TARGET cbound-game POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E create_symlink
	${RESOURCE_INPUT_DIR}
	${RESOURCE_OUTPUT_DIR}
	)
