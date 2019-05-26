#include <cmath>

#include <ssm/transform.hpp>

#include "ecs/world.hpp"

#include "core/input/input.hpp"
#include "core/script/script.hpp"

#include "debug/interface.hpp"
#include "debug/profiling.hpp"


bool show_debug = false;

int main() {
  auto window = render::create_context();
  render::init(window);
  input::init(window);

  glClearColor(0.1f, 0.2f, 0.5f, 1.0f);

  auto lua = script::new_environment();

  {
    fw::World world(window);
    world.register_functions(lua);
    world.load_scene(lua, lua.script_file(path::install_dir() /
                                          "resources/scripts/scene1.lua"));

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

      world.update(dt);

      if (show_debug) {
        debug::interface::new_frame();
        debug::add_time_sample(dt);
        debug::show_profile_window();
        debug::interface::issue_draw_call();
      }
      glfwSwapBuffers(window);
    }
  }
  input::shutdown(window);
  glfwDestroyWindow(window);
  debug::interface::shutdown();
  render::shutdown();
  return 0;
}
