#pragma once

#include <hash/flat_hash_map.hpp>

#include "core/render/sprite-batch.hpp"
#include "core/render/sprite.hpp"
#include "core/render/texture.hpp"
#include "core/resource/resource-cache.hpp"

#include "entity.hpp"
#include "scene.hpp"

namespace ecs {
class Renderer {
public:
  using iterator =
      typename ska::flat_hash_map<EntityId, render::Sprite>::iterator;
  using const_iterator =
      typename ska::flat_hash_map<EntityId, render::Sprite>::const_iterator;

  Renderer(const Scene& scene, ResourceCache<render::Texture>& textures)
      : scene(scene), batch(10000, textures) {}

  iterator get_sprite(EntityId id) { return sprites.find(id); }

  const_iterator get_sprite(EntityId id) const { return sprites.find(id); }

  iterator not_found() { return sprites.end(); }
  const_iterator not_found() const { return sprites.end(); }

  void submit(EntityId id, render::Sprite spr);

  void update(double dt);

private:
  const Scene& scene;
  ska::flat_hash_map<EntityId, render::Sprite> sprites;
  render::SpriteBatch batch;
};
} // namespace ecs
