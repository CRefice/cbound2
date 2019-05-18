#pragma once

#include <string>

#include "tex-coords.hpp"

namespace render {
// Like most types in the engine, this type doesn't actually
// hold all the data used to render it. It's merely a set of
// "instructions" for how to render it.
// You can render it from its data, but you need to get the texture
// yourself (from the resource id) and convert the frame to tex coords.
// This is so this type can be easily used in other contexts other than rendering,
// while the rendering portion takes care of getting hold of the right data.
// This is easily the defining type for the design of the rest of the engine.
struct Sprite
{
	// A resource ID for the sprite's texture.
	std::string texture_id;
	// The on-screen size of the sprite, in world coordinates.
	ssm::vec2 size;
	// The portion of the texture that will be used for rendering,
	// defined in pixel coordinates.
	TexFrame frame;
	// The depth layer the sprite is located on.
	// Sprites with lower depth will be drawn on top of ones with a higher one.
	float depth = 0.0f;
	// The color tint the sprite will be drawn with.
	ssm::vec4 color = ssm::vec4(1.0f);
};
}
