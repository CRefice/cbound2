find_package(OpenGL)

set(SRCS
	main.cpp
)

set(LIBS
	cbound-core
	Gooey
	${OPENGL_gl_LIBRARY}
)

add_gooey_executable(cbound-game "${SRCS}")
target_link_libraries(cbound-game "${LIBS}")
