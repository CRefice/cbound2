#pragma once

#include <optional>

#include <sol/forward.hpp>

#include "ecs/collision.hpp"

#include "framework.hpp"

namespace fw {
template <>
struct LuaTraits<ecs::Collision> {
  static std::optional<ecs::Collision> parse(const sol::table& tbl);
  static void bind(sol::table& tbl, const char* name);
};

void bind_collision_libs(sol::state_view state);
} // namespace fw
