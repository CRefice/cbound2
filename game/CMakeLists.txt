set(SRCS
	source/debug/interface.cpp
	source/debug/profiling.cpp
	source/ecs/animator.cpp
	source/ecs/input.cpp
	source/ecs/master-renderer.cpp
	source/ecs/scene.cpp
	source/ecs/sprite-renderer.cpp
	source/ecs/ui-manager.cpp
	source/framework/anim.cpp
	source/framework/input.cpp
	source/framework/sprite.cpp
	source/framework/tiles.cpp
	source/framework/world.cpp
	source/main.cpp
	source/ui/text.cpp
	source/ui/window.cpp
)

set(LIBS
	cbound-core
	imgui
	stdc++fs
	flat_hash_map
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
target_include_directories(cbound-game PRIVATE source/)
target_link_libraries(cbound-game PRIVATE "${LIBS}")
target_compile_options(cbound-game PRIVATE "${OPTS}")
target_compile_features(cbound-game PRIVATE cxx_std_17)
target_compile_definitions(cbound-game PRIVATE "SOL_CHECK_ARGUMENTS" "SOL_PRINT_ERRORS")

# Add a custom copy-resources command
set(RESOURCE_INPUT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/resources")
set(RESOURCE_OUTPUT_DIR "${CMAKE_CURRENT_BINARY_DIR}/resources")

add_custom_command(
	TARGET cbound-game POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E create_symlink
	${RESOURCE_INPUT_DIR}
	${RESOURCE_OUTPUT_DIR}
	)
