#include "core/anim/sequencer.hpp"

#include "anim.hpp"
#include "sprite.hpp"

#include "world.hpp"

namespace fw {
ecs::EntityId World::load_entity(const sol::table& tbl) {
  auto id = next_id++;
  scene.submit(id, ssm::vec2(0, 0));
  if (auto spr = tbl.get<sol::optional<sol::table>>("Sprite");
      auto maybe_sprite = render::parse_sprite(*spr)) {
    renderer.submit(id, *maybe_sprite);
  }
  if (auto anim = tbl.get<sol::optional<sol::table>>("Anim");
      auto maybe_anim = anim::parse_sequence(*anim)) {
    animator.submit(id, ::anim::Sequencer(*maybe_anim));
  }
  return id;
}

void World::update(double dt) {
  renderer.update(dt);
  animator.update(dt);
}
} // namespace fw
