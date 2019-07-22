#include <cassert>

#include <sol/sol.hpp>

#include "common/logging.hpp"

#include "framework/render.hpp"
#include "sprite-renderer.hpp"

using namespace render;

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
static inline float depth(const ssm::vec2& pos, float layer) {
  // Constrain y's to [0, 1], so that layers take precedence.
  return layer + constrain(pos.y);
}

void SpriteRenderer::submit(EntityId id, Sprite spr) {
  sprites.insert_or_assign(id, std::move(spr));
}

const Sprite* SpriteRenderer::find(EntityId id) const {
  auto it = sprites.find(id);
  return it == sprites.end() ? nullptr : &(it->second);
}

Sprite* SpriteRenderer::find(EntityId id) {
  auto it = sprites.find(id);
  return it == sprites.end() ? nullptr : &(it->second);
}

void SpriteRenderer::remove(EntityId id) { sprites.erase(id); }

void SpriteRenderer::draw_all(const Scene& scene, const Animator& anim) {
  for (const auto pair : sprites) {
    auto copy = Sprite(pair.second);
    auto id = pair.first;
    auto movement = scene.find(id);
    assert(movement != nullptr);
    auto pos = movement->pos;
    if (auto sequence = anim.find(id)) {
      copy.frame = sequence->current_value();
    }
    copy.depth = depth(pos, copy.depth);
    batch.draw(copy, pos);
  }
  batch.flush();
}

void SpriteRenderer::load_entity(const EntityId& id, sol::table& entity) {
  if (auto table = entity["sprite"]) {
    if (auto sprite = fw::LuaTraits<Sprite>::parse(table)) {
      submit(id, *sprite);
      table = sol::nil;
    } else {
      WARN_LOG("Unable to parse sprite component!");
    }
  }
}

void SpriteRenderer::bind_entity_fields(sol::usertype<EntityId>& meta) {
  meta["sprite"] = sol::property(
      [this](const EntityId& id) { return find(id); },
      [this](const EntityId& id, sol::table tbl) {
        if (auto sprite = fw::LuaTraits<Sprite>::parse(tbl)) {
          submit(id, *sprite);
        } else {
          WARN_LOG("Tried changing sprite to something other than a sprite!");
        }
      });
}
} // namespace ecs
