#include "core/script/script.hpp"

#include "common/logging.hpp"

#include "sprite.hpp"

namespace fw::render {
std::optional<::render::Sprite> parse_sprite(const sol::table& table) {
	if (table == sol::nil)
		return {};
	auto image_id = table.get<sol::optional<std::string>>("image");
	auto size = script::parse_vec2<float>(table["size"]);
	auto frame = script::parse_rect<int>(table["frame"]);
	if (!image_id || !size || !frame) {
		WARN_LOG("Invalid sprite data format");
		return {};
	}
	return ::render::Sprite{ *image_id, *size, *frame };
}
} // namespace fw::render
