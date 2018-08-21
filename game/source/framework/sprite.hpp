#pragma once

#include <optional>

#include <sol.hpp>

#include "core/render/sprite.hpp"

namespace fw::render {
std::optional<Sprite> parse_sprite(const sol::table& table);
} // namespace fw::render
