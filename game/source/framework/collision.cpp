#include <GLFW/glfw3.h>

#include "core/script/script.hpp"

#include "common/logging.hpp"

#include "collision.hpp"

using namespace ecs;

namespace fw::collision {
std::optional<Collision> parse_collision(const sol::table& table) {
  Collision coll;
  if (auto bounds = table.get<sol::optional<Rectangle<float>>>("bounds")) {
    coll.bounds = *bounds;
  } else {
    ERROR_LOG("Collision data does not contain collision bounds!");
    return coll;
  }

  coll.on_collision = table.get<sol::optional<sol::function>>("on_collision");
  coll.tag = table.get_or("tag", std::string());
  coll.solid = table.get_or("solid", true);
  return coll;
}
} // namespace fw::collision
