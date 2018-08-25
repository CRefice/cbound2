#include "common/color.hpp"
#include "common/logging.hpp"

#include "render/text-batch.hpp"

void TextBatch::draw(std::string_view text, const ssm::vec2& pos) {
	auto cursor = pos;
	for (auto str = text; !str.empty(); str.remove_prefix(1)) {
		char c = str[0];
		if (c == '\n') {
			newline(cursor, pos);
			continue;
		}
		if (std::string_view::size_type index;
				auto ctrl = text::parse_ctrl_code(str, &index)) {
			control_code(*ctrl);
			str.remove_prefix(index);
		}
		const auto& font = params.font;
		const auto it = font->char_map.find(std::string(1, c));
		if (it == font->char_map.end()) {
			WARN_LOG("Couldn't find a glyph for character {} in font {}",
					c, font->texture_id);
			continue;
		}
		const auto& metrics = it->second;
		const auto& frame = metrics.frame;
		ssm::vec2 size(frame.width(), frame.height());
		Sprite sprite = { font->texture_id, size, frame };
		batch.draw(sprite, cursor + ssm::vec2(0, metrics.vert_offset));
		cursor += ssm::vec2(size.x + params.char_spacing, 0);
		if (params.max_width && cursor.x >= *params.max_width) {
			newline(cursor, pos);
		}
	}
}

void TextBatch::newline(ssm::vec2& crs, const ssm::vec2& pos) {
	crs = ssm::vec2(pos.x, crs.y - params.line_spacing);
}

void TextBatch::control_code(const text::ControlCode& code) {
	if (code.name == "color") {
		auto maybe_color = code.argument ?
			color::parse_color(code.argument.value()) : std::nullopt;
		set_color(maybe_color.value_or(params.color));
	}
}
