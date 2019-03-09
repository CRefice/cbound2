#include "core/anim/sequencer.hpp"

#include "anim.hpp"
#include "input.hpp"
#include "sprite.hpp"

#include "world.hpp"

namespace fw {
void World::define_fw_functions(sol::state& tbl) {
  tbl["pos"] = [&](ecs::EntityId id) { return scene.get_position(id); };
}

ecs::EntityId World::load_entity(sol::state& lua, sol::table& tbl) {
	define_fw_functions(lua);
  auto id = next_id++;
  scene.submit(id, ssm::vec2(0, 0));
  if (auto spr = tbl.get<sol::optional<sol::table>>("Sprite")) {
    if (auto maybe_sprite = render::parse_sprite(*spr)) {
      renderer.submit(id, *maybe_sprite);
    }
  }
  if (auto anim = tbl.get<sol::optional<sol::table>>("Anim")) {
    if (auto maybe_anim = anim::parse_sequence(*anim)) {
      animator.submit(id, ::anim::Sequencer(*maybe_anim));
    }
  }
  if (auto context = tbl.get<sol::optional<sol::table>>("Input")) {
    if (auto maybe_context = input::parse_context(*context)) {
      input.submit(id, *maybe_context);
    }
  }
  return id;
}

void World::update(double dt) {
  animator.update(dt);
  renderer.update(dt);
}
} // namespace fw
