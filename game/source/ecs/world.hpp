#pragma once

#include <sol/forward.hpp>

#include "core/input/key.hpp"
#include "core/render/render.hpp"
#include "core/render/texture.hpp"
#include "core/resource/resource-cache.hpp"

#include "animator.hpp"
#include "input.hpp"
#include "master-renderer.hpp"
#include "scene.hpp"

namespace fw {
class World : public ::input::KeyHandler {
public:
  explicit World(::render::Context context);

  void register_functions(sol::state& tbl);

  void load_scene(sol::state& lua, const sol::table& tbl);
  ecs::EntityId load_entity(sol::state& lua, const sol::table& tbl);

  void remove(ecs::EntityId id);

  void update(double dt);

  void handle(::input::KeyEvent e) final { input.handle(e); }

private:
  ecs::Scene scene;
  ecs::MasterRenderer renderer;
  ecs::Animator animator;
  ecs::InputManager input;
};
} // namespace fw
