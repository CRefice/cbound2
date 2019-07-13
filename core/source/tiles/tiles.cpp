#include "tiles/tiles.hpp"

using render::TexFrame;

TexFrame frame_from_id(const TileSet& set, int id) {
  auto tile_pos = ssm::ivec2(id % set.size.x, set.size.y - id / set.size.x);
  auto pixel_pos = tile_pos * set.tile_size;
  return TexFrame(pixel_pos,
                  pixel_pos + ssm::ivec2(set.tile_size.x, -set.tile_size.y));
}

Rectangle<float> bounds(const TileMap& map) {
  return Rectangle(ssm::vec2(0, 0),
                   map.tile_size * ssm::vec2(map.size.x, map.size.y));
}
