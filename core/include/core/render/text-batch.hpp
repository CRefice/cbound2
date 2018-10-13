#pragma once

#include <string_view>
#include <optional>

#include "core/text/control-code.hpp"

#include "font.hpp"
#include "shader.hpp"
#include "sprite-batch.hpp"

namespace render {
struct TextDrawParams
{
	// The font to draw this text with.
	Resource<Font> font;
	// The base color to draw text with.
	ssm::vec4 color = ssm::vec4(1.0f);
	// If not empty, the renderer will go to the next line
	// if the text width exceeds this value,
	// instead of overflowing off-screen.
	std::optional<float> max_width = {};
	// Spacing between characters, in pixels.
	float char_spacing = 1;
	// Spacing between lines, in pixels.
	float line_spacing = 13;
};

// A SpriteBatch adapter that can draw text.
class TextBatch
{
public:
	// @param font: the font to draw the text with.
	// @param batch: a reference to the spritebatch to draw text to.
	// @param params: drawing parameters.
	TextBatch(SpriteBatch& batch, TextDrawParams params) :
		batch(batch), params(std::move(params)) {}

	// Draws the given string at the given world position.
	void draw(std::string_view str, const ssm::vec2& pos);

private:
	void control_code(const text::ControlCode& code);
	void draw_glyph(const CharMetrics& metrics, const ssm::vec2& pos);
	std::optional<CharMetrics> metrics_of(std::string_view ch);
	void set_color(ssm::vec4 clr) {
		color = std::move(clr);
	}

	SpriteBatch& batch;
	TextDrawParams params;
	ssm::vec4 color;
};
}
