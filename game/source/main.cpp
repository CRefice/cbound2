#include <cmath>

#include <ssm/transform.hpp>

#include "core/render/post-process.hpp"
#include "core/render/render-common.hpp"
#include "core/render/sprite-batch.hpp"

#include "core/resource/load-shader.hpp"
#include "core/resource/load-texture.hpp"
#include "core/resource/resource-cache.hpp"
#include "core/script/common.hpp"

#include <GLFW/glfw3.h>

#include "common/logging.hpp"

#include "framework/sprite.hpp"

constexpr int HEIGHT = 180;
constexpr int WIDTH = 320;

int main() {
	auto window = render::create_context();
	render::init(window);

	ResourceCache<shader::Stage> shaders([](const auto& id) { return load_shader(to_path(id)); });
	ResourceCache<Texture> textures([](const auto& id) { return load_texture(to_path(id)); });

	auto lua = script::new_environment();
	lua.script_file(path::install_dir() / "resources/scripts/sprite.lua");
	auto maybe_sprite = framework::parse_sprite(lua["Sprite"]);
	if (!maybe_sprite) {
		ERROR_LOG("Unable to parse sprite");
		std::abort();
	}
	auto sprt = framework::to_sprite(*maybe_sprite, [&](const auto& id) { return textures.load(id); });

	auto frag = shaders.load("shaders/sprite.f.glsl");
	auto vert = shaders.load("shaders/sprite.v.glsl");
	shader::Program program(*frag, *vert);
	auto loc = glGetUniformLocation(program.handle(), "view_projection");

	auto ppvert = shaders.load("shaders/post.v.glsl");
	auto ppfrag = shaders.load("shaders/post.f.glsl");
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);

	PostProcessStack post_process(ssm::ivec2(width, height));
	post_process.emplace_pass(
			Texture(ssm::ivec2(WIDTH, HEIGHT), nullptr),
			shader::Program(*ppvert, *ppfrag));

	SpriteBatch batch(6);
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
