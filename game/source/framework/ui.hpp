#pragma once

#include <sol/forward.hpp>

#include "ui/text.hpp"
#include "ui/widget.hpp"
#include "ui/window.hpp"

#include "framework.hpp"

namespace fw {
template <>
struct LuaTraits<ui::Widget> {
  static void bind(sol::table& tbl, const char* name);
};

template <>
struct LuaTraits<ui::Text> {
  static void bind(sol::table& tbl, const char* name);
};

template <>
struct LuaTraits<ui::Window> {
  static void bind(sol::table& tbl, const char* name);
};

void bind_ui_libs(sol::state_view state);
} // namespace fw
