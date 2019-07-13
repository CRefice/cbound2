#include <cmath>

#include "collision/collision.hpp"

namespace collision {
static ssm::ivec2 floor(const ssm::vec2& a) {
  return ssm::ivec2(std::floor(a.x), std::floor(a.y));
}

static ssm::ivec2 ceil(const ssm::vec2& a) {
  return ssm::ivec2(std::ceil(a.x), std::ceil(a.y));
}

static ssm::ivec2 invert_y(ssm::ivec2 a, const ssm::ivec2& size) {
  a.y = size.y - a.y;
  return a;
}

static Rectangle<int> subtile_rect_from_id(const TileSet& set, int id) {
  auto tile_pos = ssm::ivec2(id % set.size.x, id / set.size.x);
  auto subtile_pos = tile_pos * set.collision.subtile_res;
  return Rectangle(subtile_pos, subtile_pos + set.collision.subtile_res);
}

static Rectangle<int> tile_rect_from_rect(const TileMap& map,
                                          const Rectangle<float>& rect) {
  auto start_coord = floor(rect.bottom_left() / map.tile_size);
  auto end_coord = ceil(rect.top_right() / map.tile_size);
  return Rectangle(invert_y(start_coord, map.size),
                   invert_y(end_coord, map.size));
}

bool tile_collision(const Rectangle<float>& rect, const TileMap& map,
                       const TileSet& set) {
  auto start_tile_coord = ssm::ivec2(rect.pos() / map.tile_size);
  auto start_tile_pos = ssm::vec2(start_tile_coord) * map.tile_size;
  auto overlap_rect = tile_rect_from_rect(map, rect);
  auto subtile_size = map.tile_size / ssm::vec2(set.collision.subtile_res);

  for (const auto& layer : map.layers) {
    auto view = MatrixView(layer.tiles, overlap_rect);
    for (auto x = 0ul; x < view.width(); ++x) {
      for (auto y = 0ul; y < view.height(); ++y) {
        auto tile_coord =
            invert_y(ssm::ivec2(x, y), view.size()) - ssm::ivec2(0, 1);
        auto tile = view[tile_coord];
        if (tile == 0) {
          continue;
        }
        tile--;

        auto tile_pos = start_tile_pos + ssm::vec2(x, y) * map.tile_size;
        auto tile_rect =
            Rectangle(ssm::vec2(tile_pos), ssm::vec2(tile_pos + map.tile_size));

        auto tile_inter = *collision::intersection(tile_rect, rect);

        // Move to tile_based coordinates
        tile_inter = translate(tile_inter, -tile_pos);
        auto subtile_rel_start = floor(tile_inter.bottom_left() / subtile_size);
        auto subtile_rel_end = ceil(tile_inter.top_right() / subtile_size);

        auto sub_rect = subtile_rect_from_id(set, tile);

        auto sub_start = sub_rect.bottom_left() +
                         invert_y(subtile_rel_start, set.collision.subtile_res);
        auto sub_end = sub_rect.bottom_left() +
                       invert_y(subtile_rel_end, set.collision.subtile_res);

        sub_rect = Rectangle(sub_start, sub_end);
        auto sub_view = MatrixView(set.collision.data, sub_rect);

        if (std::any_of(sub_view.begin(), sub_view.end(),
                        [](const auto& i) { return (bool)i; })) {
          return true;
        }
      }
    }
  }
  return false;
}
} // namespace collision
