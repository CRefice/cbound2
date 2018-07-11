set(SRCS
	rendering/glinterface/gltexture.cpp
	rendering/glinterface/glprogram.cpp
	rendering/render-common.cpp
	rendering/shader.cpp
	rendering/sprite-renderer.cpp
	resource/texture.cpp
	resource/shader.cpp
)

set(LIBS
	gl-core
	stb-image
	tinyxml2
	cbound-common
)

add_cbound_library(cbound-core "${SRCS}" "${LIBS}")
