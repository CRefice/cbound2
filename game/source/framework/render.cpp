#include "core/common/logging.hpp"

#include "render.hpp"

namespace framework {
std::optional<SpriteComponent> parse_sprite(const sol::table& table) {
	if (table == sol::nil) return {};
	auto image_id = sprite.get<sol::optional<std::string>>("image");
	auto size = parse_vec2<int>(sprite["size"]);
	auto frame = parse_rect<int>(sprite["frame"]);
	if (image_id.empty() || size.empty() || frame.empty()) {
		return {};
	}
	return { *image_id, *size, *frame };
}
} // namespace framework
