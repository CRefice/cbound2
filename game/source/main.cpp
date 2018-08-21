#include <cmath>

#include <ssm/transform.hpp>

#include "core/anim/sequencer.hpp"

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
	auto loc = glGetUniformLocation(program.handle(), "view_projection");

	auto ppvert = shaders.load("shaders/post.v.glsl");
	auto ppfrag = shaders.load("shaders/post.f.glsl");
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);

	PostProcessStack post_process(ssm::ivec2(width, height));
	post_process.emplace_pass(
			Texture(ssm::ivec2(WIDTH, HEIGHT), nullptr),
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

	ResourceCache<Texture> textures([](const auto& id) { return load_texture(to_path(id)); });
	SpriteBatch batch(6, textures);

	ResourceCache<Font> fonts([](const auto& id) { return load_font(to_path(id)); });
	SpriteBatch text_spr(1000, textures);
	auto font = fonts.load("fonts/font.fnt");
	TextDrawParams params;
	params.baseline = ssm::vec2(0, 50);
	TextBatch text(font, text_spr, params);

	double old_time = glfwGetTime();
	while (!glfwWindowShouldClose(window)) {
		double new_time = glfwGetTime();
		double dt = new_time - old_time;
		old_time = new_time;

		post_process.new_frame();

		glUseProgram(program.handle());
		auto proj = ssm::ortho<float>(WIDTH, HEIGHT, 0, 100);
		glUniformMatrix4fv(loc, 1, GL_FALSE, ssm::data_ptr(proj));

		sequencer.progress(dt);
		sprt.frame = sequencer.current_frame();

		batch.draw(sprt, ssm::vec2(-30, 0));
		batch.flush();

		text.draw(R"(the quick brown fox
		jumps over the lazy dog
		hi. yeah, but;
		like this: what! really?
		'char' "str" oh_man
		$(bash)[index] 1+1-1*2/2
		echo hi | val = 2 <kek> 2^3%4
		101&010)");
		text.clear();

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
