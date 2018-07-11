#pragma once

#include "common/vec2.hpp"
#include "common/inttypes.hpp"

#include "texture.hpp"

struct TileSet
{
	Texture texture;
	vec2<uint> tile_size;
	vec2<uint> size;
};
