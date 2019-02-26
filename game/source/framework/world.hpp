#pragma once

#include <sol.hpp>

#include "core/render/texture.hpp"
#include "core/resource/resource-cache.hpp"

#include "ecs/animator.hpp"
#include "ecs/renderer.hpp"
#include "ecs/scene.hpp"

namespace fw {
class World {
public:
  World(ResourceCache<::render::Texture>& textures)
      : renderer(scene, textures), animator(renderer) {}

  ecs::EntityId load_entity(const sol::table& tbl);
	void update(double dt);

private:
  ecs::EntityId next_id = 0ul;
  ecs::Scene scene;
  ecs::Renderer renderer;
  ecs::Animator animator;
};
} // namespace fw
