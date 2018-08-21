#include <cstdlib> //abort()

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <ssm/vector.hpp>

#include "common/logging.hpp"

#include "render/render-common.hpp"

namespace render
{
Context create_context() {
	SCOPE_LOG("Creating an OpenGL context");

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#ifndef NDEBUG
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
	glfwSetErrorCallback([](int error, const char* description) {
			ERROR_LOG("GLFW error #{}: {}", error, description);
			});
#endif

	GLFWwindow* window = glfwCreateWindow(800, 600, "Test", nullptr, nullptr);
	if (window == nullptr) {
		FATAL_LOG("Failed to create an OpenGL context");
		glfwTerminate();
		std::abort();
	}
	return window;
}

void init(Context context) {
	SCOPE_LOG("Initializing main rendering pipeline");

	glfwMakeContextCurrent(context);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		FATAL_LOG("Failed to load OpenGL function pointers");
		std::abort();
	}
#ifndef NDEBUG
	if (GLAD_GL_ARB_debug_output) {
		SCOPE_LOG("Enabling debug output");
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS_ARB);
		glDebugMessageCallbackARB([](GLenum source, GLenum type, GLuint id, GLenum severity,
					GLsizei length, const GLchar* message, const void*) {
				auto level = type == GL_DEBUG_TYPE_ERROR_ARB ? logging::Error : logging::Info;
				logging::shared_log().log(level, "OpenGL: {}", std::string_view(message, length));
				}, nullptr);
	}
#endif
	glViewport(0, 0, 800, 600);
}

void shutdown() {
	SCOPE_LOG("Shutting down rendering pipeline");
	glfwTerminate();
}
} // namespace render
