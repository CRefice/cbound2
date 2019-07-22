#pragma once

#include <optional>

#include <sol/forward.hpp>

#include "core/render/sprite.hpp"

#include "framework.hpp"

namespace fw {
template <>
struct LuaTraits<render::Sprite> {
  static std::optional<render::Sprite> parse(const sol::table& table);
  static void bind(sol::table& table, const char* name);
};

void bind_render_libs(sol::state_view state);
} // namespace fw
