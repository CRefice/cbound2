set(SRCS
	source/anim/text-drawl.cpp
	source/common/paths.cpp
	source/common/string.cpp
	source/input/input.cpp
	source/render/framebuffer.cpp
	source/render/post-process.cpp
	source/render/render.cpp
	source/render/shader.cpp
	source/render/sprite-batch.cpp
	source/render/text-batch.cpp
	source/render/texture.cpp
	source/render/tiles.cpp
	source/render/tile-batch.cpp
	source/render/uniform-buffer.cpp
	source/resource/load-font.cpp
	source/resource/load-shader.cpp
	source/resource/load-tileset.cpp
	source/resource/load-tilemap.cpp
	source/resource/load-texture.cpp
	source/script/script.cpp
	source/text/control-codes.cpp
)

# Choose OpenGL vendor implementation by default
if(POLICY CMP0072)
  cmake_policy(SET CMP0072 NEW)
endif()
find_package(OpenGL REQUIRED)

set(LIBS
	fmt
	glad
	glfw
	sol2
	ssm
	pugixml
	stb-image
	flat_hash_map
	${OPENGL_gl_LIBRARY}
)

set(CMAKE_CXX_STANDARD 17)

add_library(cbound-core ${SRCS})
target_include_directories(cbound-core PUBLIC include)
target_include_directories(cbound-core PRIVATE include/core)
target_link_libraries(cbound-core PUBLIC ${LIBS})
