#include <cmath>

#include <ssm/transform.hpp>

#include "ecs/world.hpp"

#include "common/logging.hpp"

#include "core/input/input.hpp"
#include "core/script/script.hpp"
#include "core/script/scheduler.hpp"

#include "editor/tiles.hpp"

#include "debug/interface.hpp"
#include "debug/profiling.hpp"

bool show_debug = true;

int main() {
  auto window = render::create_context();
  render::init(window);
  input::ActionQueue queue;

  auto config = load_resource<input::InputConfig>(to_path("config/input.xml"));
  input::InputManager input(window, queue, std::move(config));

  glClearColor(0.1f, 0.2f, 0.5f, 1.0f);

  auto lua = script::new_environment();
	script::CoroutineScheduler<ecs::EntityId> sched(lua);

  {
    ecs::World world(window, sched, queue);
    world.bind_libs(lua);

    auto maybe_scene = lua.safe_script_file(path::install_dir() /
                                                "resources/scripts/scene1.lua",
                                            script::on_error)
                           .get<sol::optional<sol::table>>();

    ResourceCache<::render::Texture> textures;
    ResourceCache<TileSet> tilesets;

    if (!maybe_scene) {
      FATAL_LOG("Unable to load main scene");
      std::abort();
    }
    world.load_scene(*maybe_scene);

    debug::interface::init(window);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glfwSwapInterval(1);
    double old_time = glfwGetTime();
    ssm::vec2 pos(0, 20);
    while (!glfwWindowShouldClose(window)) {
      glfwPollEvents();

      double new_time = glfwGetTime();
      double dt = new_time - old_time;
      old_time = new_time;

      input.poll();

      world.update(dt);
			sched.tick_all();

      if (show_debug) {
        debug::interface::new_frame();
        debug::add_time_sample(dt);
        debug::show_profile_window();
        editor::tileset_editor(tilesets, textures);
        debug::interface::issue_draw_call();
      }
      glfwSwapBuffers(window);
    }
  }
  glfwDestroyWindow(window);
  debug::interface::shutdown();
  render::shutdown();
  return 0;
}
