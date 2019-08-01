#pragma once

#include <sol/forward.hpp>

#include "core/input/action.hpp"
#include "core/script/scheduler.hpp"

#include "animator.hpp"
#include "behavior.hpp"
#include "collision.hpp"
#include "input.hpp"
#include "master-renderer.hpp"
#include "scene.hpp"

namespace ecs {
class World {
public:
  World(render::GLContext context, BehaviorScheduler& sched,
        input::ActionQueue& queue);

  void load_scene(sol::table& tbl);
  ecs::EntityId load_entity(sol::table& tbl);
  void remove(ecs::EntityId id);

  void bind_libs(sol::state_view state);

  void update(double dt);

private:
  void bind_entity_table(sol::table& tbl);

  double time = 0.0;

  ecs::Scene scene;
  ecs::MasterRenderer renderer;
  ecs::BehaviorManager behavior;
  ecs::BehaviorRunner runner;
  ecs::InputManager input;
  ecs::CollisionManager collision;

  std::vector<ecs::EntityId> remove_list;

  ResourceCache<TileMap> tile_maps;
  ResourceCache<TileSet> tile_sets;
};
} // namespace ecs
