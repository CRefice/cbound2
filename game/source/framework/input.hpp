#pragma once

#include <optional>

#include <sol.hpp>

#include "core/input/key.hpp"
#include "core/input/context.hpp"

namespace fw::input {
std::optional<::input::Context> parse_context(const sol::table& table);
} // namespace fw::render
