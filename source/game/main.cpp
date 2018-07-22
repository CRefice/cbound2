#include <algorithm>
#include <iostream>

#include <fmt/core.h>
#include <ssm/transform.hpp>

#include "common/logging.hpp"
#include "common/paths.hpp"

#include "render/render-common.hpp"
#include "render/sprite-batch.hpp"

#include "resource/load-shader.hpp"
#include "resource/load-texture.hpp"

#include <GLFW/glfw3.h>

int loc;

int main() {
	auto window = render::create_context();
	render::init(window);
	glClearColor(0.2f, 0.3f, 0.7f, 1.0f);

	shader::Program program(load_shader((path::install_dir() / "resources/shaders/sprite.f.glsl").string()),
			load_shader((path::install_dir() / "resources/shaders/sprite.v.glsl").string()));
	glUseProgram(program.handle());
	loc = glGetUniformLocation(program.handle(), "view_projection");

	glfwSetFramebufferSizeCallback(window, [](GLFWwindow* wnd, int width, int height) {
			glViewport(0,0,width,height);
			ssm::mat4 proj = ssm::ortho<float>(width, height, 0, 100);
			glUniformMatrix4fv(loc, 1, GL_FALSE, ssm::data_ptr(proj.data[0]));
			});


	glActiveTexture(GL_TEXTURE0);
	Texture tex = load_texture((path::install_dir() / "resources/textures/sprite.png").string());
	Sprite sprt = {
		Resource<Texture>("bla", &tex),
		ssm::vec2(100, 100),
		{ {0, 0xFFFF}, {0xFFFF, 0} }
	};

	SpriteBatch batch(6);

	while (!glfwWindowShouldClose(window)) {
		glClear(GL_COLOR_BUFFER_BIT);

		batch.draw(sprt, ssm::vec2(100, 100));
		batch.draw(sprt, ssm::vec2(0, 100));
		batch.draw(sprt, ssm::vec2(50, 50));
		batch.draw(sprt, ssm::vec2(200, 100));
		batch.draw(sprt, ssm::vec2(0, 50));
		batch.draw(sprt, ssm::vec2(100, 0));
		batch.flush();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	render::shutdown();
	return 0;
}
