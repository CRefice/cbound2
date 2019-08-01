#include <sol/sol.hpp>
#include <ssm/transform.hpp>

#include "common/logging.hpp"

#include "framework/anim.hpp"
#include "framework/collision.hpp"
#include "framework/render.hpp"

#include "world.hpp"

namespace ecs {
World::World(::render::GLContext context, BehaviorScheduler& sched,
             input::ActionQueue& queue)
    : renderer(context), runner(sched), input(queue) {}

void World::bind_entity_table(sol::table& tbl) {
  auto meta = tbl.new_usertype<EntityId>("Entity", sol::no_constructor);
  meta["pos"] = sol::property(
      [this](const EntityId& id) { return scene.find(id)->pos; },
      [this](const EntityId& id, ssm::vec2 val) { scene.find(id)->pos = val; });
  meta["vel"] = sol::property(
      [this](const EntityId& id) { return scene.find(id)->velocity; },
      [this](const EntityId& id, ssm::vec2 val) {
        scene.find(id)->velocity = val;
      });
  renderer.bind_entity_fields(meta);
}

void World::bind_libs(sol::state_view state) {
  fw::bind_anim_libs(state);
  fw::bind_render_libs(state);
  fw::bind_collision_libs(state);
  renderer.bind_libs(state);

  auto table = state.create_table("world");
  table["instantiate"] = [this](sol::table table) { load_entity(table); };
  table["remove"] = [this](sol::table table) { remove(table["id"]); };
  table["game_time"] = [this] { return time; };
  bind_entity_table(table);
}

void World::load_scene(sol::table& tbl) {
  auto entities = tbl.get<sol::optional<sol::table>>("entities");
  auto map_path = tbl.get<sol::optional<std::string>>("tile_map");
  auto set_path = tbl.get<sol::optional<std::string>>("tile_set");
  if (!map_path || !set_path || !entities) {
    WARN_LOG("Parsing scene failed: not all required components are present!");
    return;
  }

  scene = ecs::Scene();
  scene.tile_map = tile_maps.load(*map_path);
  scene.tile_set = tile_sets.load(*set_path);
  renderer.switch_tiles(*scene.tile_map, *scene.tile_set);
  for (auto& entry : *entities) {
    sol::table entity = entry.second;
    load_entity(entity);
  }
}

EntityId World::load_entity(sol::table& tbl) {
  auto pos = tbl.get_or("position", ssm::vec2(0.0f));
  auto id = scene.submit(ecs::Movement{pos, ssm::vec2(0.0f)});
  tbl["id"] = id;
  {
    auto meta = tbl[sol::metatable_key];
    if (!meta) {
      meta = tbl.create_named("meta");
    }
    meta[sol::meta_function::index] = tbl["id"];
    meta[sol::meta_function::new_index] = tbl["id"];
  }
  renderer.load_entity(id, tbl);
  behavior.load_entity(id, tbl);
  collision.load_entity(id, tbl);
  input.load_entity(id, tbl);
  runner.submit(id, tbl);

  return id;
}

void World::remove(EntityId id) { remove_list.push_back(std::move(id)); }

void World::update(double dt) {
  while (!remove_list.empty()) {
    auto id = remove_list.back();

    scene.remove(id);
    renderer.remove(id);
    collision.remove(id);
    behavior.remove(id);
    runner.remove(id);
    input.remove(id);

    remove_list.pop_back();
  }

  input.dispatch(runner);

  scene.update(dt);
  collision.update(dt, scene, runner);
  renderer.update(dt);
  behavior.update(dt, runner);

  renderer.draw_all(scene);
  time += dt;
}
} // namespace ecs
