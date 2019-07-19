#include <sol/sol.hpp>
#include <ssm/transform.hpp>

#include "common/logging.hpp"
#include "core/anim/sequencer.hpp"
#include "core/input/action.hpp"
#include "core/script/script.hpp"

#include "framework/anim.hpp"
#include "framework/collision.hpp"
#include "framework/input.hpp"
#include "framework/sprite.hpp"
#include "framework/ui.hpp"

#include "world.hpp"

namespace ecs {
World::World(::render::Context context, input::ActionQueue& queue)
    : renderer(context, animator), input(queue) {}

void World::register_functions(sol::state_view state) {
  auto meta =
      state.new_usertype<EntityId>("Entity", "new", sol::no_constructor);
  meta["pos"] =
      sol::property([this](const EntityId& id) { return scene.find(id)->pos; });
  meta["vel"] = sol::property(
      [this](const EntityId& id) { return scene.find(id)->velocity; },
      [this](const EntityId& id, ssm::vec2 val) {
        scene.find(id)->velocity = val;
      });

  state.create_named_table(
      "world", "instantiate",
      [this](sol::table table) { return load_entity(table); }, "remove",
      [this](const ecs::EntityId& id) { remove(id); }, "game_time",
      [this]() { return time; });

  fw::anim::register_libs(state);
  animator.register_fields(meta);
  renderer.load_libraries(state);
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
  auto default_pos = ssm::vec2{};
  auto pos =
      tbl.get<sol::optional<ssm::vec2>>("position").value_or(default_pos);
  auto id = scene.submit(ecs::Movement{pos, ssm::vec2(0, 0)});
  tbl["id"] = id;
  {
    auto meta = tbl[sol::metatable_key];
    if (!meta) {
      meta = tbl.create_named("meta");
    }
    meta[sol::meta_function::index] = tbl["id"];
    meta[sol::meta_function::new_index] = tbl["id"];
  }
  animator.load_entity(id, tbl);
  if (auto spr = tbl.get<sol::optional<sol::table>>("sprite")) {
    if (auto maybe_sprite = fw::render::parse_sprite(*spr)) {
      renderer.submit(id, *maybe_sprite);
    }
  }
  if (auto behavior = tbl.get<sol::optional<sol::function>>("behavior")) {
    updates.submit(id, Closure{tbl, *behavior});
  }
  if (auto coll = tbl["collision"]) {
    if (auto maybe_coll = fw::collision::parse_collision(tbl, coll)) {
      collision.submit(id, *maybe_coll);
    }
  }
  if (auto context = tbl["input"]) {
    if (auto maybe_context = fw::input::parse_context(tbl, context)) {
      input.submit(id, *maybe_context);
    }
  }
  if (auto widget = tbl["ui"]) {
    if (auto maybe_widget = fw::ui::parse_widget(widget)) {
      renderer.submit(id, std::move(maybe_widget));
    }
  }

  return id;
}

void World::remove(EntityId id) { remove_list.push_back(std::move(id)); }

void World::update(double dt) {
  while (!remove_list.empty()) {
    auto id = remove_list.back();

    scene.remove(id);
    renderer.remove(id);
    collision.remove(id);
    updates.remove(id);
    animator.remove(id);
    input.remove(id);

    remove_list.pop_back();
  }

  input.dispatch(behav);

  scene.update(dt);
  collision.update(dt, scene, behav);
  updates.tick_all(behav);
  animator.update(dt);
  renderer.update(dt);
  renderer.draw_all(scene);
  behav.tick_all();
  time += dt;
}
} // namespace ecs
