#include <sol/sol.hpp>

#include "common/logging.hpp"

#include "animator.hpp"
#include "framework/anim.hpp"

using namespace anim;

namespace ecs {
using Sequencer = Animator::Sequencer;

void Animator::submit(EntityId id, Sequencer anim) {
  anims.insert_or_assign(id, std::move(anim));
}

Sequencer* Animator::find(EntityId id) {
  auto it = anims.find(id);
  return it == anims.end() ? nullptr : &(it->second);
}

const Sequencer* Animator::find(EntityId id) const {
  auto it = anims.find(id);
  return it == anims.end() ? nullptr : &(it->second);
}

void Animator::remove(EntityId id) { anims.erase(id); }

void Animator::update(double dt) {
  for (auto& elem : anims) {
    elem.second.progress(dt);
  }
}

void Animator::load_entity(const EntityId& id, sol::table& entity) {
  if (auto anim = entity["animation"]) {
    if (auto sequencer = fw::LuaTraits<Sequencer>::parse(anim)) {
      submit(id, std::move(*sequencer));
      anim = sol::nil;
    } else {
      WARN_LOG("Unable to parse animation component!");
    }
  }
}

void Animator::bind_entity_fields(sol::usertype<EntityId>& meta) {
  meta["animation"] = sol::property(
      [this](const EntityId& id) { return find(id); },
      [this](const EntityId& id, sol::table tbl) {
        if (auto sequencer = fw::LuaTraits<Sequencer>::parse(tbl)) {
					// Why does this REQUIRE a move?! Investigate
          submit(id, std::move(*sequencer));
        } else {
          WARN_LOG("Tried switching to something other than an animation!");
        }
      });
}
} // namespace ecs
