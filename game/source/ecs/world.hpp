#pragma once

#include <sol/forward.hpp>

#include "core/input/key.hpp"

#include "animator.hpp"
#include "behavior.hpp"
#include "collision.hpp"
#include "input.hpp"
#include "master-renderer.hpp"
#include "scene.hpp"

namespace ecs {
class World : public ::input::KeyHandler {
public:
  explicit World(::render::Context context);

  void register_functions(sol::state_view state);

  void load_scene(sol::table& tbl);
  ecs::EntityId load_entity(sol::table& tbl);

  void remove(ecs::EntityId id);

  void update(double dt);

  void handle(::input::KeyEvent e) final { input.handle(e); }

private:
  double time = 0.0;

  ecs::Scene scene;
  ecs::MasterRenderer renderer;
  ecs::Animator animator;
  ecs::InputManager input;
  ecs::BehaviorManager behav;
  ecs::UpdateManager updates;
  ecs::CollisionManager collision;

  std::vector<ecs::EntityId> remove_list;

  ResourceCache<TileMap> tile_maps;
  ResourceCache<TileSet> tile_sets;
};
} // namespace ecs
