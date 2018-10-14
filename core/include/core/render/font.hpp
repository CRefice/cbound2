#pragma once

#include <string>
#include <unordered_map>

#include "tex-coords.hpp"

namespace render {
// Various metrics for a single character of a font.
struct CharMetrics
{
	// The rectangle, in pixel coordinates, that encloses the glyph.
	TexFrame frame;
	// How many pixels the glyph "dips" below the baseline.
	int vert_offset;
};

// A collection of sprites that represent characters,
// held in a char -> metrics map.
// Note that chars are utf-8 encoded,
// and as such may be more than one byte long...
// we thus need to use a string. Hurray.
struct Font
{
	std::string texture_id;
	std::unordered_map<std::string, CharMetrics> char_map;
};
}
