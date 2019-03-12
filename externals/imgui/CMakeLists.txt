set(SRCS
	imgui/imgui.cpp
	imgui/examples/imgui_impl_glfw.cpp
	imgui/examples/imgui_impl_opengl3.cpp
	imgui/imgui_demo.cpp
	imgui/imgui_draw.cpp
	imgui/imgui_widgets.cpp
)
add_library(imgui "${SRCS}")
target_link_libraries(imgui PUBLIC glad glfw)
target_include_directories(imgui PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(imgui PRIVATE imgui)
target_include_directories(imgui PUBLIC imgui/examples)
target_compile_definitions(imgui PUBLIC IMGUI_IMPL_OPENGL_LOADER_GLAD)
