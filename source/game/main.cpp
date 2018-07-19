#include <algorithm>
#include <iostream>

#include "common/paths.hpp"
#include "common/logging.hpp"
#include "render/render-common.hpp"

#include <GLFW/glfw3.h>
#include <fmt/core.h>

int main() {
	auto window = render::create_context();
	render::init(window);
	glClearColor(0.2f, 0.3f, 0.7f, 1.0f);
	glfwSetFramebufferSizeCallback(window, [](GLFWwindow* wnd, int width, int height) {
			glViewport(0,0,width,height);
			});
	while (!glfwWindowShouldClose(window)) {
		glClear(GL_COLOR_BUFFER_BIT);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	render::shutdown();
	return 0;
}
