#include "render/tiles.hpp"

namespace render {
TexFrame frame_from_id(const TileSet& set, int id) {
	auto tile_pos = ssm::ivec2(id % set.size.x, id / set.size.y);
	auto pixel_pos = tile_pos * set.tile_size;
	return TexFrame(pixel_pos, pixel_pos + set.tile_size);
}

Rectangle<float> bounds(const TileMap& map) {
	return Rectangle(ssm::vec2(0,0),
			map.tile_size * ssm::vec2(map.size.x, map.size.y));
}
}
