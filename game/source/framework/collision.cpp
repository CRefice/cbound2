#include <GLFW/glfw3.h>

#include "core/script/script.hpp"

#include "common/logging.hpp"

#include "collision.hpp"

using namespace ecs;

namespace fw::collision {
std::optional<Collision> parse_collision(const sol::table& table) {
  Collision coll;
  auto bounds = table.get<sol::optional<sol::table>>("bounds");
  if (!bounds) {
    ERROR_LOG("Collision data does not contain required attributes!");
    return coll;
  }
  if (auto rect = script::parse_rect<float>(*bounds)) {
    coll.bounds = *rect;
  } else {
    ERROR_LOG("Couldn't parse collision bounds data!");
    return coll;
  }

  coll.on_collision = table.get<sol::optional<sol::function>>("on_collision");
  coll.solid = table.get_or("solid", true);
  return coll;
}
} // namespace fw::collision
