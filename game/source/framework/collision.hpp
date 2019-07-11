#pragma once

#include <optional>

#include <sol/forward.hpp>

#include "ecs/collision.hpp"

namespace fw::collision {
std::optional<ecs::Collision> parse_collision(const sol::table& table);
} // namespace fw::collision
