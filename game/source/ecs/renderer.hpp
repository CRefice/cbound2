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
  Renderer(const Scene& scene, ResourceCache<render::Texture>& textures)
      : scene(scene), batch(10000, textures) {}

  render::Sprite* find(EntityId id);
  const render::Sprite* find(EntityId id) const;

  void submit(EntityId id, render::Sprite spr);

  void update(double dt);

private:
  const Scene& scene;
  ska::flat_hash_map<EntityId, render::Sprite> sprites;
  render::SpriteBatch batch;
};
} // namespace ecs
