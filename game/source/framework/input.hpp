#pragma once

#include <optional>

#include <sol/forward.hpp>

#include "core/input/key.hpp"
#include "ecs/input.hpp"

namespace fw::input {
std::optional<ecs::InputContext> parse_context(const sol::table& tbl, const sol::table& table);
} // namespace fw::render
