#include <sol/sol.hpp>

#include "common/logging.hpp"

#include "collision.hpp"

namespace fw {
using namespace ecs;
using Traits = LuaTraits<Collision>;

std::optional<Collision> Traits::parse(const sol::table& table) {
  Collision coll;
  if (auto bounds = table.get<sol::optional<Rectangle<float>>>("bounds")) {
    coll.bounds = *bounds;
  } else {
    WARN_LOG("Collision data does not contain collision bounds!");
    return coll;
  }

  if (auto fn = table.get<sol::optional<sol::function>>("on_collision")) {
    coll.on_collision = fn;
  }
  coll.tag = table.get_or("tag", std::string());
  coll.solid = table.get_or("solid", true);
  return coll;
}

void Traits::bind(sol::table& tbl, const char* name) {
  auto meta = tbl.new_usertype<Collision>(name, sol::no_constructor);
  meta["bounds"] = &Collision::bounds;
  meta["on_collision"] = &Collision::on_collision;
  meta["tag"] = &Collision::tag;
  meta["solid"] = &Collision::solid;
}

void bind_collision_libs(sol::state_view state) {
  auto table = state.create_table("collision");
  Traits::bind(table, "Collision");
}
} // namespace fw
