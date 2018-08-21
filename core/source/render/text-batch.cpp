#include "common/logging.hpp"

#include "render/text-batch.hpp"

void TextBatch::draw(std::string_view str) {
	for (char c : str) {
		if (c == '\n') {
			newline();
			continue;
		}
		const auto it = font->char_map.find(std::string(1, c));
		if (it == font->char_map.end()) {
			WARN_LOG("Couldn't find a glyph for character {} in font {}",
					c, font->texture_id);
			continue;
		}
		const auto& metrics = it->second;
		auto frame = metrics.frame;
		ssm::vec2 size(frame.width(), frame.height());
		Sprite sprite = { font->texture_id, size, frame };
		batch.draw(sprite, cursor + ssm::vec2(0, metrics.vert_offset));
		cursor += ssm::vec2(frame.width() + params.char_spacing, 0);
		if (params.max_width && cursor.x >= *params.max_width) {
			newline();
		}
	}
}

void TextBatch::clear() {
	cursor = params.baseline;
	batch.flush();
}

void TextBatch::newline() {
	cursor.x = (float)params.baseline.x;
	cursor.y = cursor.y - params.line_spacing;
}
