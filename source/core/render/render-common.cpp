#include <cstdlib> //abort()

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <ssm/vector.hpp>

#include "common/logging.hpp"

#include "render-common.hpp"

namespace render
{
/*
constexpr vec2<uint> buffer_size{ 320, 180 };

static GLuint render_fbo;
static GLuint render_texture;
static GLuint quad_vao;
static Resource<Shader> pp_shader;

static vec2<uint> viewport_pos, viewport_size;

static void setup_vao() {
	GLuint vbo, ebo;
	GLfloat vertices[] = {
		//Position		//Texcoord
		 1.0f,  1.0f,	1.0f, 1.0f,
		 1.0f, -1.0f,	1.0f, 0.0f,
		-1.0f, -1.0f,	0.0f, 0.0f,
		-1.0f,  1.0f,	0.0f, 1.0f
	};
	GLuint indices[] = {
		0, 1, 3,
		1, 2, 3
	};

	glGenVertexArrays(1, &quad_vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ebo);

	glBindVertexArray(quad_vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)(2 * sizeof(float)));

	glBindVertexArray(0);
}

static void setup_fbo(GLuint& fbo, GLuint& tex, vec2<uint> size) {
	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, size.x, size.y, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex, 0);

	GLuint rbo;
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 800, 600);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		ERROR_LOG("Framebuffer creation: framebuffer is not complete!");
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
*/

Context create_context() {
	DEBUG_LOG("Creating an OpenGL context...");
	glfwInit();
	glfwSetErrorCallback([](int error, const char* description) {
			ERROR_LOG("GLFW error #{}: {}", error, description);
			});

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "Test", nullptr, nullptr);
	if (window == nullptr) {
		FATAL_LOG("Failed to create an OpenGL context");
		glfwTerminate();
		std::abort();
	}
	DEBUG_LOG("Finished creating OpenGL context");
	return window;
}

void init(Context context) {
	DEBUG_LOG("Initializing main rendering pipeline");
	glfwMakeContextCurrent(context);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		FATAL_LOG("Failed to load OpenGL function pointers");
		std::abort();
	}
	glViewport(0, 0, 800, 600);
	DEBUG_LOG("Finished initializing rendering pipeline");
}

void shutdown() {
	DEBUG_LOG("Shutting down rendering pipeline");
	glfwTerminate();
	DEBUG_LOG("Finished shutting down rendering pipeline");
}
} // namespace render
