#pragma once

#include <cstdint>
#include <vector>

#include <hash/flat_hash_map.hpp>

#include "common/matrix.hpp"
#include "core/anim/sequence.hpp"
#include "tex-coords.hpp"

namespace render {
enum class TileCollisionType : std::uint8_t { None, Full };

struct TileSet {
  std::string texture_id;
  // Number of tiles in both directions
  ssm::ivec2 size;
  // Size of a single tile, in pixel coordinates
  PixelCoord tile_size;
  // Maps tile-id to their animation sequence
  ska::flat_hash_map<int, anim::Sequence<int>> anim_tiles;
  // Contains each tile's collision type
  Matrix<TileCollisionType> collisions;
};

TexFrame frame_from_id(const TileSet& set, int id);

struct TileMap {
  struct Layer {
    // Drawing depth, in the range [1,inf]
    int depth;
    // Vector of tile IDs
    std::vector<int> tiles;
  };
  // Number of tiles in both directions
  ssm::ivec2 size;
  // Size of a single tile, in world coordinates.
  ssm::vec2 tile_size;
  std::vector<Layer> layers;
};

Rectangle<float> bounds(const TileMap& map);
} // namespace render
