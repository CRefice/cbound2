#pragma once

#include <optional>

#include <sol.hpp>

#include "core/render/sprite.hpp"

namespace framework {
std::optional<Sprite> parse_sprite(const sol::table& table);
} // namespace framework
