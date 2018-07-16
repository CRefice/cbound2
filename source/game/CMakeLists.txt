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
