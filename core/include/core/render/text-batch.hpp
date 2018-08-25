#pragma once

#include <string_view>
#include <optional>

#include "core/text/control-codes.hpp"

#include "font.hpp"
#include "shader.hpp"
#include "sprite-batch.hpp"

struct TextDrawParams
{
	// The font to draw this text with.
	Resource<Font> font;
	// The base color to draw text with.
	ssm::vec3 color = ssm::vec3(1, 1, 1);
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
		batch(batch), params(std::move(params)), color(params.color) {}

	// Draws the given string at position pos.
	void draw(std::string_view str, const ssm::vec2& pos);

	void set_color(ssm::vec3 clr) {
		color = std::move(clr);
	}

private:
	void newline(ssm::vec2& cursor, const ssm::vec2& pos);
	void control_code(const text::ControlCode& code);

	SpriteBatch& batch;
	TextDrawParams params;
	ssm::vec3 color;
};
