#include <algorithm>

#include <Gooey/Include/App.hpp>
#include <Gooey/Include/OpenGL/GLWindow.hpp>

#include <common/path-util.hpp>

#include <core/rendering/sprite-renderer.hpp>
#include <core/rendering/render-common.hpp>

#include <core/resource/resource-cache.hpp>
#include <core/resource/texture.hpp>
#include <core/resource/shader.hpp>

int main() {
	bool master_run = true;
	Goo::GLWindow window;
	window.setText("Test");
	window.setClientArea({ 640, 360 });
	window.onResize.setHandler([&](Goo::Size) {
		static const Goo::Size viewport_size = { 320, 180 };
		const Goo::Size size = window.clientArea();
		const float ratio = std::min(float(size.x) / viewport_size.x, float(size.y) / viewport_size.y);
		Goo::Size actual_size = { int(viewport_size.x * ratio), int(viewport_size.y * ratio) };

		Goo::Size blank_area = size - actual_size;
		Goo::Point pos{ blank_area.x / 2, blank_area.y / 2 };
		rendercommon::set_viewport({ pos.x, pos.y }, { actual_size.x, actual_size.y });
	});
	window.onDestroy.setHandler([&]() {
		master_run = false;
	});
	window.setVisibleOnCreate(false);
	window.create();
	rendercommon::init(window);
	rendercommon::set_viewport({ 0, 0 }, { window.clientArea().x, window.clientArea().y });

	window.setCursorVisibility(false);
	window.setFullscreen(true);

	path::setup_paths();

	ResourceCache<ShaderLoader> shader_cache;
	rendercommon::set_postprocess_shader(shader_cache.load({
				path::user_path(DIR_SHADERS) + "draw_postprocess.vs",
				path::user_path(DIR_SHADERS) + "draw_postprocess.fs"
				});

	sprite_renderer::init(shader_cache.load({
			path::user_path(DIR_SHADERS) + "sprite.vs",
			path::user_path(DIR_SHADERS) + "sprite.fs"
			});

	ResourceCache<TextureLoader> texm;
	auto texres = texm.load(path::user_path(DIR_TEXTURES) + "Travis.png");
	Sprite sprite{ texres };

	while (master_run) {
		Goo::App::pollEvents();

		rendercommon::begin_draw();
		sprite_renderer::draw(sprite, { 0, 10 });
		rendercommon::end_draw();
		window.swapBuffers();
	}

	return 0;
}
