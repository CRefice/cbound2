#pragma once

#include <string_view>
#include <optional>

#include "font.hpp"
#include "sprite-batch.hpp"

struct TextDrawParams
{
	// Where to start drawing the text.
	ssm::vec2 baseline;
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
	TextBatch(Resource<Font> font,
			SpriteBatch& batch, TextDrawParams params) :
		font(font), batch(batch), params(std::move(params)),
	cursor(params.baseline) {}

	// Draws the given string into the provided SpriteBatch,
	// using the provided TextParams.
	void draw(std::string_view str);

	// Resets the cursor to baseline position,
	// and flushes the underlying SpriteBatch.
	void clear();

private:
	void newline();

	Resource<Font> font;
	SpriteBatch& batch;
	TextDrawParams params;
	ssm::vec2 cursor;
};
