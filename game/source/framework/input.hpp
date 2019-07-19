#pragma once

#include <optional>

#include <sol/forward.hpp>

#include "ecs/input.hpp"

namespace fw::input {
std::optional<ecs::InputContext> parse_context(const sol::table& tbl, sol::table table);
} // namespace fw::render
