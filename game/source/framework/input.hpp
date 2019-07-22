#pragma once

#include <optional>

#include <sol/forward.hpp>

#include "ecs/input.hpp"
#include "framework.hpp"

namespace fw {
template <>
struct LuaTraits<ecs::ActionMap> {
  static std::optional<ecs::ActionMap> parse(const sol::table& tbl);
};
} // namespace fw
