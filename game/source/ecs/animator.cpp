#include <cassert>

#include "animator.hpp"

namespace ecs {
void Animator::submit(EntityId id, Sequencer anim) {
  anims.insert_or_assign(id, std::move(anim));
}

void Animator::update(double dt) {
  for (auto &[id, anim] : anims) {
    anim.progress(dt);
    auto sprite = renderer.find(id);
    // Every entity with an Anim component should also have a Sprite component
    assert(sprite != nullptr);
    sprite->frame = anim.current_value();
  }
}
}
