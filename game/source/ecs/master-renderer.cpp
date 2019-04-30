#include "master-renderer.hpp"

inline int SCREEN_HEIGHT = 180;
inline int SCREEN_WIDTH = 320;

namespace ecs {
MasterRenderer::MasterRenderer(render::Context context, const Animator& anim)
    : sprite_renderer(anim, textures), post_process(context) {
  auto frag = shaders.load("shaders/sprite.f.glsl");
  auto vert = shaders.load("shaders/sprite.v.glsl");
  sprite_shader = shader::Program(*frag, *vert);
  sprite_shader.bind("Camera", camera_buf);
  frag = shaders.load("shaders/tile.f.glsl");
  vert = shaders.load("shaders/tile.v.glsl");
  tile_shader = shader::Program(*frag, *vert);
  tile_shader.bind("Camera", camera_buf);
  frag = shaders.load("shaders/post.f.glsl");
  vert = shaders.load("shaders/post.v.glsl");

  post_process.emplace_pass(
      render::Texture(ssm::ivec2(SCREEN_WIDTH, SCREEN_HEIGHT), nullptr),
      shader::Program(*vert, *frag));
}

void MasterRenderer::switch_tiles(const render::TileMap& map,
                                  const render::TileSet& set) {
  static_tiles = render::StaticTileBatch(textures, map, set);
  dynamic_tiles = render::AnimTileBatch(textures, map, set);
}

void MasterRenderer::draw_all(const Scene& scene, const Camera& camera) {
  camera_buf.store<Camera>(camera);

	post_process.new_frame();

  glUseProgram(tile_shader.handle());
  static_tiles.issue_draw_call();
  dynamic_tiles.issue_draw_call(scene.tile_set);

  sprite_renderer.draw_all(scene, sprite_shader);

	post_process.draw_all();
}
} // namespace ecs
