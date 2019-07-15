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

Sequencer *Animator::find(EntityId id) {
  auto it = anims.find(id);
  return it == anims.end() ? nullptr : &(it->second);
}

const Sequencer *Animator::find(EntityId id) const {
  auto it = anims.find(id);
  return it == anims.end() ? nullptr : &(it->second);
}

void Animator::remove(EntityId id) { anims.erase(id); }

void Animator::update(double dt) {
  for (auto &[id, anim] : anims) {
    anim.progress(dt);
  }
}

void Animator::load_libraries(sol::state &state) {
  auto table = state.create_table_with(
      "switch_to", [this](const EntityId &id, const sol::table &tbl) {
        if (auto sequence = fw::anim::parse_sequence(tbl)) {
          auto [old_it, inserted] = anims.emplace(id, *sequence);
          if (!inserted) {
            old_it->second.switch_to(std::move(*sequence));
          }
        } else {
          WARN_LOG("Tried switching to something other than an animation!");
        }
      });
	state["anim"] = table;

  // Sequence types
  table.new_enum("play_mode", "once", PlayMode::Once, "loop", PlayMode::Loop,
                 "ping_pong", PlayMode::PingPong);
}
}
