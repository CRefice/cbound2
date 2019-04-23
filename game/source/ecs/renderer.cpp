#include "renderer.hpp"

namespace ecs {
void Renderer::submit(EntityId id, render::Sprite spr) {
  sprites.insert_or_assign(id, std::move(spr));
}

const render::Sprite* Renderer::find(EntityId id) const {
  auto it = sprites.find(id);
  return it == sprites.end() ? nullptr : &(it->second);
}

render::Sprite* Renderer::find(EntityId id) {
  auto it = sprites.find(id);
  return it == sprites.end() ? nullptr : &(it->second);
}

void Renderer::update(double dt) {
  for (const auto &[id, sprite] : sprites) {
    auto movement = scene.find(id);
    if (movement) {
      batch.draw(sprite, movement->pos);
    }
  }
  batch.flush();
}
}
