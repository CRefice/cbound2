#include <cassert>

#include "sprite-renderer.hpp"

namespace ecs {
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

void SpriteRenderer::draw_all(const Scene &scene,
                              const shader::Program &shader) {
  glUseProgram(shader.handle());
  for (const auto &[id, s] : sprites) {
		render::Sprite sprite{s};
    auto movement = scene.find(id);
    assert(movement != nullptr);
		if (auto sequence = anim.find(id)) {
			sprite.frame = sequence->current_value();
		}
    batch.draw(sprite, movement->pos);
  }
  batch.flush();
}
}
