#include <cmath>

#include <ssm/transform.hpp>

#include "render/post-process.hpp"
#include "render/render-common.hpp"
#include "render/sprite-batch.hpp"

#include "resource/load-shader.hpp"
#include "resource/load-texture.hpp"
#include "resource/resource-cache.hpp"

#include <GLFW/glfw3.h>

constexpr int HEIGHT = 180;
constexpr int WIDTH = 320;

int main() {
	auto window = render::create_context();
	render::init(window);
	glClearColor(0.3f, 0.2f, 0.4f, 1.0f);

	ResourceCache<shader::Stage> shaders([](const auto& id) { return load_shader(to_path(id)); });
	auto frag = shaders.load("shaders/sprite.f.glsl");
	auto vert = shaders.load("shaders/sprite.v.glsl");
	shader::Program program(*frag, *vert);
	auto loc = glGetUniformLocation(program.handle(), "view_projection");

	glActiveTexture(GL_TEXTURE0);
	ResourceCache<Texture> textures([](const auto& id) { return load_texture(to_path(id)); });
	Sprite sprt = {
		textures.load("textures/sprite.png"),
		ssm::vec2(40, 40),
		{ {0, 0}, {0xFFFF, 0xFFFF} }
	};

	SpriteBatch batch(6);

	auto ppvert = shaders.load("shaders/post.v.glsl");
	auto ppfrag = shaders.load("shaders/post.f.glsl");
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);

	PostProcessStack post_process(ssm::ivec2(width, height));
	post_process.emplace_pass(
			Texture(ssm::ivec2(WIDTH, HEIGHT), nullptr),
			shader::Program(*ppvert, *ppfrag));

	double offset = 0;
	while (!glfwWindowShouldClose(window)) {
		post_process.new_frame();

		glUseProgram(program.handle());
		auto proj = ssm::ortho<float>(WIDTH, HEIGHT, 0, 100);
		glUniformMatrix4fv(loc, 1, GL_FALSE, ssm::data_ptr(proj));

		batch.draw(sprt, ssm::vec2(10, 10));
		batch.draw(sprt, ssm::vec2(0, 10));
		batch.draw(sprt, ssm::vec2(5, 5));
		batch.draw(sprt, ssm::vec2(20, 10));
		batch.draw(sprt, ssm::vec2(0, 5));
		batch.draw(sprt, ssm::vec2(10, 0));
		batch.flush();
		offset += 0.2;

		post_process.draw_all();
		glfwSwapBuffers(window);

		glfwPollEvents();
		int width, height;
		glfwGetFramebufferSize(window, &width, &height);
		post_process.set_screen_size({width, height});
	}
	render::shutdown();
	return 0;
}
