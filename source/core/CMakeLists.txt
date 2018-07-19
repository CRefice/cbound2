set(SRCS
	common/paths.cpp
	render/render-common.cpp
	render/sprite-batch.cpp
	render/texture.cpp
	resource/load-texture.cpp
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
	ssm
	stb-image
	${OPENGL_gl_LIBRARY}
)

set(CMAKE_CXX_STANDARD 17)

add_library(cbound-core ${SRCS})
target_include_directories(cbound-core PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(cbound-core PUBLIC ${LIBS})
