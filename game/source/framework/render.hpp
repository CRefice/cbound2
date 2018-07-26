#pragma once

#include <optional>
#include <string>

#include <sol.hpp>

#include "core/common/rectangle.hpp"
#include "core/render/sprite.hpp"

namespace framework {
struct SpriteComponent
{
	std::string image_id;
	ssm::ivec2 size;
	Rectangle<int> frame;
};

std::optional<SpriteComponent> parse_sprite(const sol::table& table);
} // namespace framework
