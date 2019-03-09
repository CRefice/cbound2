#pragma once

#include <sol.hpp>

#include "core/input/key.hpp"
#include "core/render/render.hpp"
#include "core/render/texture.hpp"
#include "core/resource/resource-cache.hpp"

#include "ecs/animator.hpp"
#include "ecs/input.hpp"
#include "ecs/renderer.hpp"
#include "ecs/scene.hpp"

namespace fw {
class World : public ::input::KeyHandler {
public:
  World(ResourceCache<::render::Texture>& textures)
      : renderer(scene, textures), animator(renderer) {}

  ecs::EntityId load_entity(sol::state& lua, sol::table& tbl);
  void update(double dt);

  void handle(::input::KeyEvent e) final { input.handle(e); }

private:
  void define_fw_functions(sol::state& tbl);
  ecs::EntityId next_id = 0ul;
  ecs::Scene scene;
  ecs::Renderer renderer;
  ecs::Animator animator;
  ecs::InputManager input;
};
} // namespace fw
