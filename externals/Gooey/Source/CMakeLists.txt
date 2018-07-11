if (WIN32)
	set (PLATFORM_DIR Win)
else ()
	set (PLATFORM_DIR Gtk)
endif ()

file (GLOB Sources "${PLATFORM_DIR}/*.cpp")

find_package (OpenGL)
if (OPENGL_FOUND)
option (GOOEY_BUILD_OPENGL "Build the OpenGL platform abstraction layer" ON)
endif ()

if (GOOEY_BUILD_OPENGL)
	file(GLOB GLSources "${PLATFORM_DIR}/OpenGL/*.cpp")
endif()

add_library (Gooey ${Sources} ${GLSources})
target_include_directories(Gooey PRIVATE "${PROJECT_SOURCE_DIR}/Include")

install(TARGETS Gooey DESTINATION Lib)
