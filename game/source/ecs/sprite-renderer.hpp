#pragma once

#include <hash/flat_hash_map.hpp>

#include "core/render/shader.hpp"
#include "core/render/sprite-batch.hpp"
#include "core/render/sprite.hpp"
#include "core/render/texture.hpp"
#include "core/resource/resource-cache.hpp"

#include "animator.hpp"
#include "entity.hpp"
#include "scene.hpp"

namespace ecs {
class SpriteRenderer {
public:
  SpriteRenderer(const Animator& anim, ResourceCache<render::Texture>& textures)
      : anim(anim), batch(10000, textures) {}

  render::Sprite* find(EntityId id);
  const render::Sprite* find(EntityId id) const;

  void submit(EntityId id, render::Sprite spr);
  void remove(EntityId id);

  void draw_all(const Scene& scene);

private:
  const Animator& anim;
  ska::flat_hash_map<EntityId, render::Sprite> sprites;
  render::SpriteBatch batch;
};
} // namespace ecs
