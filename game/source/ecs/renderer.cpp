#include "renderer.hpp"

namespace ecs {
void Renderer::submit(EntityId id, render::Sprite spr) {
	sprites.insert_or_assign(id, std::move(spr));
}

void Renderer::update(double dt) {
	for (const auto& [id, sprite] : sprites) {
		auto pos = scene.get_position(id);
		batch.draw(sprite, pos);
	}
	batch.flush();
}
}
