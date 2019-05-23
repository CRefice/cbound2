#include "common/logging.hpp"
#include <ssm/transform.hpp>

#include "core/anim/sequencer.hpp"
#include "core/input/input.hpp"

#include "ui/text.hpp"

#include "anim.hpp"
#include "input.hpp"
#include "sprite.hpp"
#include "tiles.hpp"
#include "ui.hpp"

#include "world.hpp"

inline int SCREEN_HEIGHT = 180;
inline int SCREEN_WIDTH = 320;

namespace fw {
World::World(::render::Context context) : renderer(context, animator) {
  ::input::set_handler(context, *this);
  camera.view = ssm::identity<float, 4>();
  camera.projection =
      ssm::ortho<float>(0, SCREEN_WIDTH, SCREEN_HEIGHT, 0, 0, -100);
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
      "remove", [this](const ecs::EntityId& id) { remove(id); });

  anim::load_libraries(tbl);
  renderer.load_libraries(tbl);
}

void World::load_scene(sol::state& lua, const sol::table& tbl) {
  auto tile_map = tiles::parse_tilemap(tbl["tile_map"]);
  auto tile_set = tiles::parse_tileset(tbl["tile_set"]);
  auto entities = tbl.get<sol::optional<sol::table>>("entities");
  if (!tile_map || !tile_set || !entities) {
    WARN_LOG("Parsing scene failed: not all required components are present!");
    return;
  }

  scene = ecs::Scene();
  scene.tile_map = *tile_map;
  scene.tile_set = *tile_set;
  renderer.switch_tiles(*tile_map, *tile_set);
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
    if (auto maybe_sprite = render::parse_sprite(*spr)) {
      renderer.submit(id, *maybe_sprite);
    }
  }
  if (auto anim = tbl.get<sol::optional<sol::table>>("anim")) {
    if (auto maybe_anim = anim::parse_sequence(*anim)) {
      animator.submit(id, ::anim::Sequencer(*maybe_anim));
    }
  }
  if (auto context = tbl.get<sol::optional<sol::table>>("input")) {
    if (auto maybe_context = input::parse_context(*context)) {
      input.submit(id, *maybe_context);
    }
  }
  if (auto widget = tbl.get<::ui::Widget*>("ui")) {
    renderer.submit(id, widget);
  }
  return id;
}

void World::remove(ecs::EntityId id) {
  scene.remove(id);
  animator.remove(id);
  input.remove(id);
  renderer.remove(id);
}

void World::update(double dt) {
  scene.update(dt);
  animator.update(dt);
  renderer.update(dt);
  renderer.draw_all(scene, camera);

	input.delete_dead();
}
} // namespace fw
