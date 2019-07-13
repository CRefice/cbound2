#include <sol/sol.hpp>
#include <ssm/transform.hpp>

#include "master-renderer.hpp"

inline int SCREEN_HEIGHT = 180;
inline int SCREEN_WIDTH = 320;

namespace ecs {
using namespace ::render;

MasterRenderer::MasterRenderer(Context context, const Animator& anim)
    : sprite_renderer(anim, textures), ui(textures), post_process(context) {
  auto frag = shaders.load("shaders/sprite.f.glsl");
  auto vert = shaders.load("shaders/sprite.v.glsl");
  sprite_shader = shader::Program(*frag, *vert);
  vert = shaders.load("shaders/ui.v.glsl");
  ui_shader = shader::Program(*frag, *vert);
  sprite_shader.bind("Camera", camera_buf);
  frag = shaders.load("shaders/tile.f.glsl");
  vert = shaders.load("shaders/tile.v.glsl");
  tile_shader = shader::Program(*frag, *vert);
  tile_shader.bind("Camera", camera_buf);
  frag = shaders.load("shaders/post.f.glsl");
  vert = shaders.load("shaders/post.v.glsl");

  post_process.emplace_pass(
      Texture(ssm::ivec2(SCREEN_WIDTH, SCREEN_HEIGHT), nullptr),
      shader::Program(*vert, *frag));

  camera.view = ssm::identity<float, 4>();
  camera.projection =
      ssm::ortho<float>(0, SCREEN_WIDTH, SCREEN_HEIGHT, 0, 0, -100);
}

void MasterRenderer::update(double dt) {
  dynamic_tiles.progress(dt);
  ui.update(dt);
}

void MasterRenderer::remove(EntityId id) {
  sprite_renderer.remove(id);
  ui.remove(id);
}

void MasterRenderer::switch_tiles(const TileMap& map, const TileSet& set) {
  static_tiles = StaticTileBatch(textures, map, set);
  dynamic_tiles = AnimTileBatch(textures, map, set);
}

void MasterRenderer::draw_all(const Scene& scene) {
  camera_buf.store<Camera>(camera);

  post_process.new_frame();

  glUseProgram(tile_shader.handle());
  dynamic_tiles.issue_draw_call(*scene.tile_set);
  static_tiles.issue_draw_call();

  glUseProgram(sprite_shader.handle());
  sprite_renderer.draw_all(scene);

  glUseProgram(ui_shader.handle());
  ui.draw_all(scene);

  post_process.draw_all();
}

void MasterRenderer::load_libraries(sol::state& state) {
  ui.load_libraries(state);

  auto table = state.create_table("render");
  table.create_named("camera", "pan_to", [this](ssm::vec2 position) {
    auto vec = ssm::extend(position, 0.0);
    camera.view = ssm::translation(vec);
  });
}
} // namespace ecs
