#include <sol/sol.hpp>
#include <ssm/transform.hpp>

#include "common/logging.hpp"
#include "core/anim/sequencer.hpp"
#include "core/input/input.hpp"
#include "core/script/script.hpp"

#include "ui/text.hpp"

#include "framework/anim.hpp"
#include "framework/collision.hpp"
#include "framework/input.hpp"
#include "framework/sprite.hpp"
#include "framework/ui.hpp"

#include "world.hpp"

namespace ecs {
World::World(::render::Context context)
    : renderer(context, animator), input(behav) {
  ::input::set_handler(context, *this);
}

void World::register_functions(sol::state& tbl) {
  tbl.new_usertype<ecs::EntityId>(
      "Entity", "pos",
      sol::property([&](ecs::EntityId& id) { return scene.find(id)->pos; }),
      "vel",
      sol::property([&](ecs::EntityId& id) { return scene.find(id)->velocity; },
                    [&](ecs::EntityId& id, ssm::vec2 val) {
                      scene.find(id)->velocity = val;
                    }),
      "new", sol::no_constructor);

  tbl.create_named_table(
      "world", "instantiate",
      [this](const sol::table& table, sol::this_state s) {
        return load_entity((sol::state&)s, table);
      },
      "remove", [this](const ecs::EntityId& id) { remove(id); }, "game_time",
      [this]() { return time; });

  animator.load_libraries(tbl);
  renderer.load_libraries(tbl);
}

void World::load_scene(sol::state& lua, const sol::table& tbl) {
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
  for (auto [name, entity] : *entities) {
    load_entity(lua, entity);
  }
}

ecs::EntityId World::load_entity(sol::state& lua, const sol::table& tbl) {
  auto default_pos = ssm::vec2{};
  auto pos =
      tbl.get<sol::optional<ssm::vec2>>("position").value_or(default_pos);
  auto id = scene.submit(ecs::Movement{pos, ssm::vec2(0, 0)});
  if (auto spr = tbl.get<sol::optional<sol::table>>("sprite")) {
    if (auto maybe_sprite = fw::render::parse_sprite(*spr)) {
      renderer.submit(id, *maybe_sprite);
    }
  }
  if (auto behavior = tbl.get<sol::optional<sol::function>>("behavior")) {
    updates.submit(id, *behavior);
  }
  if (auto coll = tbl["collision"]) {
    if (auto maybe_coll = fw::collision::parse_collision(coll)) {
      collision.submit(id, *maybe_coll);
    }
  }
  if (auto anim = tbl["anim"]) {
    if (auto maybe_anim = fw::anim::parse_sequence(anim)) {
      animator.submit(id, ::anim::Sequencer(*maybe_anim));
    }
  }
  if (auto context = tbl["input"]) {
    if (auto maybe_context = fw::input::parse_context(context)) {
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

void World::remove(ecs::EntityId id) { remove_list.push_back(std::move(id)); }

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

  scene.update(dt);
  collision.update(dt, scene, behav);
  updates.tick_all(behav);
  animator.update(dt);
  renderer.update(dt);
  renderer.draw_all(scene);
  input.delete_dead();
  behav.tick_all();
  time += dt;
}
} // namespace ecs
