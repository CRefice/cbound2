#include <sol/sol.hpp>
#include <ssm/transform.hpp>

#include "framework/anim.hpp"

#include "master-renderer.hpp"

inline int SCREEN_HEIGHT = 180;
inline int SCREEN_WIDTH = 320;

namespace ecs {
using namespace ::render;

MasterRenderer::MasterRenderer(Context context)
    : sprite_renderer(textures), ui(textures), post_process(context) {
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
  anim.update(dt);
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
  sprite_renderer.draw_all(scene, anim);

  glUseProgram(ui_shader.handle());
  ui.draw_all(scene);

  post_process.draw_all();
}

void MasterRenderer::load_entity(const EntityId& id, sol::table& tbl) {
  anim.load_entity(id, tbl);
  sprite_renderer.load_entity(id, tbl);
  ui.load_entity(id, tbl);
}

void MasterRenderer::bind_libs(sol::state_view state) {
	ui.bind_libs(state);
  auto table = state.get<sol::table>("render");
  auto cam = table.create_named("camera");
  cam["move_to"] = [this](ssm::vec2 vec) {
		vec.x = std::round(vec.x);
		vec.y = std::round(vec.y);
    auto transl = ssm::extend(vec, 0.0f);
    camera.view = ssm::translation(-transl);
  };
  cam["resolution"] = ssm::vec2(SCREEN_WIDTH, SCREEN_HEIGHT);
}

void MasterRenderer::bind_entity_fields(sol::usertype<EntityId>& meta) {
  anim.bind_entity_fields(meta);
  sprite_renderer.bind_entity_fields(meta);
}
} // namespace ecs
