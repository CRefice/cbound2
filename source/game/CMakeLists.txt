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
file(GLOB_RECURSE RESOURCES ${PROJECT_SOURCE_DIR}/resources/*.*)

set(RESOURCE_OUTPUT_DIR "${CMAKE_CURRENT_BINARY_DIR}/resources")

add_custom_command(
        TARGET cbound-game POST_BUILD
				COMMAND ${CMAKE_COMMAND} -E make_directory ${RESOURCE_OUTPUT_DIR}
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
								${RESOURCES}
								${RESOURCE_OUTPUT_DIR})
