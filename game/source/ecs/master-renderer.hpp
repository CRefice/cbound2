#pragma once

#include <hash/flat_hash_map.hpp>

#include "core/render/post-process.hpp"
#include "core/render/render.hpp"
#include "core/render/tile-batch.hpp"

#include "camera.hpp"
#include "sprite-renderer.hpp"

namespace ecs {
class MasterRenderer {
public:
  MasterRenderer(render::Context context, const Animator& anim);

  render::Sprite* find(EntityId id) { return sprite_renderer.find(id); }

  const render::Sprite* find(EntityId id) const {
    return sprite_renderer.find(id);
  }

  void submit(EntityId id, render::Sprite spr) {
    sprite_renderer.submit(id, spr);
  }

  void update(double dt) { dynamic_tiles.progress(dt); }
  void switch_tiles(const render::TileMap& map, const render::TileSet& set);

  void draw_all(const Scene& scene, const Camera& camera);

private:
  render::UniformBuffer camera_buf{sizeof(Camera)};
  ResourceCache<shader::Stage> shaders;
  ResourceCache<render::Texture> textures;

  SpriteRenderer sprite_renderer;
  render::StaticTileBatch static_tiles;
  render::AnimTileBatch dynamic_tiles;
  render::PostProcessStack post_process;
  shader::Program sprite_shader;
  shader::Program tile_shader;
};
} // namespace ecs
