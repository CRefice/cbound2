#include <cmath>

#include <ssm/transform.hpp>

#include "core/anim/sequencer.hpp"
#include "core/anim/text-drawl.hpp"

#include "core/input/context.hpp"
#include "core/input/input.hpp"

#include "core/render/post-process.hpp"
#include "core/render/render.hpp"
#include "core/render/sprite-batch.hpp"
#include "core/render/text-batch.hpp"
#include "core/render/tile-batch.hpp"

#include "core/resource/load-file.hpp"
#include "core/resource/resource-cache.hpp"
#include "core/script/script.hpp"

#include <GLFW/glfw3.h>

#include "common/logging.hpp"

#include "debug/interface.hpp"
#include "debug/profiling.hpp"

#include "framework/anim.hpp"
#include "framework/input.hpp"
#include "framework/sprite.hpp"
#include "framework/tiles.hpp"

inline int HEIGHT = 180;
inline int WIDTH = 320;

bool show_debug = false;

void init_input(render::Context window) {
  using namespace input;
  auto& stack = input::get_context(window);
  KeyEvent debug{GLFW_KEY_Q, input::KeyEvent::Action::Press};
  Context debug_ctx;
  debug_ctx.actions[debug] = [] { show_debug = !show_debug; };
  stack.push(debug_ctx);
}

int main() {
  auto window = render::create_context();
  render::init(window);
  input::init(window);
  init_input(window);

  glClearColor(0.1f, 0.2f, 0.5f, 1.0f);

  ResourceCache<shader::Stage> shaders(
      [](const auto& id) { return load_shader(to_path(id)); });
  auto frag = shaders.load("shaders/sprite.f.glsl");
  auto vert = shaders.load("shaders/sprite.v.glsl");
  shader::Program program(*frag, *vert);
  auto debug_vert = shaders.load("shaders/debug.v.glsl");
  auto debug_frag = shaders.load("shaders/debug.f.glsl");
  shader::Program debug_program(*debug_frag, *debug_vert);
  auto tile_frag = shaders.load("shaders/tile.f.glsl");
  auto tile_vert = shaders.load("shaders/tile.v.glsl");
  shader::Program tile_program(*tile_frag, *tile_vert);

  auto ppvert = shaders.load("shaders/post.v.glsl");
  auto ppfrag = shaders.load("shaders/post.f.glsl");
  int width, height;
  glfwGetFramebufferSize(window, &width, &height);

  render::PostProcessStack post_process(ssm::ivec2(width, height));
  post_process.emplace_pass(render::Texture(ssm::ivec2(WIDTH, HEIGHT), nullptr),
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
  auto maybe_input = fw::input::parse_context(bird["Input"]);
  if (!maybe_input) {
    ERROR_LOG("Unable to parse input");
    std::abort();
  }
  auto& stack = input::get_context(window);
  stack.push(*maybe_input);

  auto sequencer = anim::Sequencer(*maybe_sequence);

  auto res = lua.script_file(path::install_dir() /
                             "resources/tilesets/overworld-set.lua");
  auto tile_set = fw::tiles::parse_tileset(res);
  res = lua.script_file(path::install_dir() /
                        "resources/tilemaps/overworld-map.lua");
  auto tile_map = fw::tiles::parse_tilemap(res);

  ResourceCache<render::Texture> textures(
      [](const auto& id) { return load_texture(to_path(id)); });
  render::SpriteBatch batch(10000, textures);
  render::SpriteBatch debug_batch(10000, textures);
  render::SpriteBatch text_batch(10000, textures);
  render::StaticTileBatch tile_batch(textures, *tile_map, *tile_set);
  render::AnimTileBatch anim_tile_batch(textures, *tile_map, *tile_set);

  ResourceCache<render::Font> fonts(
      [](const auto& id) { return load_font(to_path(id)); });
  auto font = fonts.load("fonts/font.fnt");
  render::TextDrawParams params{font};
  render::TextBatch text(text_batch, params);

  auto str = R"(Woah, I can do {color:#ff2020}colored{color} text!)";
  anim::TextDrawl text_anim(str);

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

    pos += ssm::vec2(bird["x"], bird["y"]) * (float)(100 * dt);

    auto proj = ssm::ortho<float>(0, WIDTH, HEIGHT, 0, 0, -100);
    glUseProgram(tile_program.handle());
    anim_tile_batch.progress(dt);
    anim_tile_batch.issue_draw_call();
    tile_program.uniform("view_projection") = proj;
    tile_batch.issue_draw_call();

    glUseProgram(program.handle());
    program.uniform("view_projection") = proj;

    sequencer.progress(dt);
    sprt.frame = sequencer.current_value();

    batch.draw(sprt, pos);
    batch.flush();
    if (show_debug) {
      glUseProgram(debug_program.handle());
      debug_program.uniform("view_projection") = proj;
      debug_batch.draw(sprt, pos);
      debug_batch.flush();
    }

    glUseProgram(program.handle());

    text_anim.progress(dt);
    text.draw(text_anim.current_slice(), ssm::vec2(100, 50));
    text_batch.flush();

    post_process.draw_all();

    if (show_debug) {
      debug::interface::new_frame();
      debug::add_time_sample(dt);
      debug::show_profile_window();
      debug::interface::issue_draw_call();
    }
    glfwSwapBuffers(window);

    post_process.new_frame();

    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    post_process.set_screen_size({width, height});
  }
  input::shutdown(window);
  glfwDestroyWindow(window);
  debug::interface::shutdown();
  render::shutdown();
  return 0;
}
