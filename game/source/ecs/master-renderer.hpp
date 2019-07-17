#pragma once

#include <hash/flat_hash_map.hpp>
#include <sol/forward.hpp>

#include "core/render/post-process.hpp"
#include "core/render/render.hpp"
#include "core/render/tile-batch.hpp"

#include "ui/widget.hpp"

#include "camera.hpp"
#include "sprite-renderer.hpp"
#include "ui-manager.hpp"

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

  void submit(EntityId id, std::unique_ptr<ui::Widget> widget) {
    ui.submit(id, std::move(widget));
  }

  void remove(EntityId id);

  void update(double dt);
  void switch_tiles(const TileMap& map, const TileSet& set);

  void draw_all(const Scene& scene);

  void load_libraries(sol::state_view state);

private:
  render::UniformBuffer camera_buf{sizeof(Camera)};
  ResourceCache<shader::Stage> shaders;
  ResourceCache<render::Texture> textures;

  SpriteRenderer sprite_renderer;
  UiManager ui;

  ecs::Camera camera;
  render::StaticTileBatch static_tiles;
  render::AnimTileBatch dynamic_tiles;
  render::PostProcessStack post_process;
  shader::Program sprite_shader;
  shader::Program tile_shader;
  shader::Program ui_shader;
};
} // namespace ecs
