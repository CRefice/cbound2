#pragma once

#include <optional>
#include <string>

#include <sol.hpp>

#include "common/rectangle.hpp"
#include "core/render/sprite.hpp"

namespace framework {
// Basically the same thing as a sprite, but without the actual resource.
struct SpriteComponent
{
	std::string image_id;
	ssm::vector<float, 2> size;
	Rectangle<int> frame;
};

std::optional<SpriteComponent> parse_sprite(const sol::table& table);

template <typename Fn>
Sprite to_sprite(const SpriteComponent& comp, Fn image_load_function) {
	Resource<Texture> tex = image_load_function(comp.image_id);
	return Sprite{ tex, comp.size, comp.frame };
}
} // namespace framework
