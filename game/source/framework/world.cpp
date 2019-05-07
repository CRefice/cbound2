#include <ssm/transform.hpp>

#include "common/logging.hpp"

#include "core/anim/sequencer.hpp"
#include "core/input/input.hpp"

#include "anim.hpp"
#include "input.hpp"
#include "tiles.hpp"
#include "sprite.hpp"

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

void World::define_fw_functions(sol::state& tbl) {
  tbl.new_usertype<ecs::EntityId>(
      "Entity", "pos",
      sol::property([&](ecs::EntityId& id) { return scene.find(id)->pos; }),
      "vel",
      sol::property([&](ecs::EntityId& id) { return scene.find(id)->velocity; },
                    [&](ecs::EntityId& id, ssm::vec2 val) {
                      scene.find(id)->velocity = val;
                    }),
      "new", sol::no_constructor);
  tbl["window"] = [this](ssm::vec2 tl, ssm::vec2 br, std::string_view text) {
    auto rect = Rectangle<float>(tl, br);
    // ui.text_window(rect, text);
  };
}

void World::load_scene(sol::state& lua, const sol::table& tbl) {
  define_fw_functions(lua);
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
  auto id = scene.submit(ecs::Movement{ssm::vec2(0, 0), ssm::vec2(0, 0)});
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
  return id;
}

void World::update(double dt) {
  scene.update(dt);
  animator.update(dt);
  renderer.update(dt);
  renderer.draw_all(scene, camera);
}
} // namespace fw
