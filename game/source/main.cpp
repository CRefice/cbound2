#include <cmath>

#include <ssm/transform.hpp>

#include "core/anim/sequencer.hpp"
#include "core/anim/text-drawl.hpp"

#include "core/render/post-process.hpp"
#include "core/render/render-common.hpp"
#include "core/render/sprite-batch.hpp"
#include "core/render/text-batch.hpp"

#include "core/resource/load-file.hpp"
#include "core/resource/resource-cache.hpp"
#include "core/script/common.hpp"

#include <GLFW/glfw3.h>

#include "common/logging.hpp"

#include "framework/sprite.hpp"
#include "framework/anim.hpp"

inline int HEIGHT = 180;
inline int WIDTH = 320;

int main() {
	auto window = render::create_context();
	render::init(window);
	glClearColor(0.1f, 0.2f, 0.5f, 1.0f);

	ResourceCache<shader::Stage> shaders([](const auto& id) { return load_shader(to_path(id)); });
	auto frag = shaders.load("shaders/sprite.f.glsl");
	auto vert = shaders.load("shaders/sprite.v.glsl");
	shader::Program program(*frag, *vert);

	auto ppvert = shaders.load("shaders/post.v.glsl");
	auto ppfrag = shaders.load("shaders/post.f.glsl");
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);

	render::PostProcessStack post_process(ssm::ivec2(width, height));
	post_process.emplace_pass(
			render::Texture(ssm::ivec2(WIDTH, HEIGHT), nullptr),
			shader::Program(*ppvert, *ppfrag));

	auto lua = script::new_environment();
	fw::anim::load_libraries(lua);
	lua.script_file(path::install_dir() / "resources/scripts/Birb.lua");
	const auto bird = lua["bird"];
	auto maybe_sprite = fw::render::parse_sprite(bird["Sprite"]);
	if (!maybe_sprite) {
		ERROR_LOG("Unable to parse sprite");
		std::abort();
	}
	auto sprt = *maybe_sprite;
	auto maybe_sequence = fw::anim::parse_sequence(bird["Animation"]["fly"]);
	if (!maybe_sequence) {
		ERROR_LOG("Unable to parse sequence");
		std::abort();
	}
	auto sequencer = anim::Sequencer(*maybe_sequence);

	ResourceCache<render::Texture> textures([](const auto& id) { return load_texture(to_path(id)); });
	render::SpriteBatch batch(200, textures);
	render::SpriteBatch text_batch(200, textures);

	ResourceCache<render::Font> fonts([](const auto& id) { return load_font(to_path(id)); });
	auto font = fonts.load("fonts/font.fnt");
	render::TextDrawParams params{ font };
	render::TextBatch text(text_batch, params);

	auto str = R"(Elena is a {color:#ffb6c1}wonderful person)";
	anim::TextDrawl text_anim(str);

	double old_time = glfwGetTime();
	while (!glfwWindowShouldClose(window)) {
		double new_time = glfwGetTime();
		double dt = new_time - old_time;
		old_time = new_time;

		post_process.new_frame();

		glUseProgram(program.handle());
		auto proj = ssm::ortho<float>(WIDTH, HEIGHT, 0, 100);
		program.uniform("view_projection") = proj;

		sequencer.progress(dt);
		sprt.frame = sequencer.current_value();

		batch.draw(sprt, ssm::vec2(-30, 0));
		batch.flush();

		text_anim.progress(dt);
		text.draw(text_anim.current_slice(), ssm::vec2(0, 50));
		text_batch.flush();

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
