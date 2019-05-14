#pragma once

#include <optional>

#include <sol/sol.hpp>

#include "core/render/sprite.hpp"

namespace fw::render {
std::optional<::render::Sprite> parse_sprite(const sol::table& table);
} // namespace fw::render
