#include <cassert>

#include "sprite-renderer.hpp"

namespace ecs {
// Logistic-like function that maps the range [-inf, inf] to [0, 1]
// The higher growth is, the slower it will reach the asymptote
static inline float constrain(float x) {
  constexpr float growth = 10000;
  auto y = x / std::sqrt(x * x + growth);
  return (y + 1.0f) / 2.0f;
}

// Add depth to a 2-d coordinate, given a layer.
// Sprites are ordered by y coordinate (by a factor of 0.01)
// Layer is also taken into account
static inline float depth(const ssm::vec2 &pos, float layer) {
  // Constrain y's to [0, 1], so that layers take precedence.
  return layer + constrain(pos.y);
}

void SpriteRenderer::submit(EntityId id, render::Sprite spr) {
  sprites.insert_or_assign(id, std::move(spr));
}

const render::Sprite *SpriteRenderer::find(EntityId id) const {
  auto it = sprites.find(id);
  return it == sprites.end() ? nullptr : &(it->second);
}

render::Sprite *SpriteRenderer::find(EntityId id) {
  auto it = sprites.find(id);
  return it == sprites.end() ? nullptr : &(it->second);
}

void SpriteRenderer::draw_all(const Scene &scene) {
  for (const auto &[id, s] : sprites) {
    render::Sprite sprite{s};
    auto movement = scene.find(id);
    assert(movement != nullptr);
    auto pos = movement->pos;
    if (auto sequence = anim.find(id)) {
      sprite.frame = sequence->current_value();
    }
    sprite.depth = depth(pos, sprite.depth);
    batch.draw(sprite, pos);
  }
  batch.flush();
}
}
