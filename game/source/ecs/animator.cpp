#include "animator.hpp"

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

void Animator::remove(EntityId id) {
  anims.erase(id);
}

void Animator::update(double dt) {
  for (auto &[id, anim] : anims) {
    anim.progress(dt);
  }
}
}
