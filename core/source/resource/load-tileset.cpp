#include <iostream>
#include <sstream>

#include <pugixml.hpp>

#include "common/logging.hpp"
#include "common/string.hpp"
#include "render/tiles.hpp"

#include "resource/resource.hpp"

namespace fs = std::filesystem;
using namespace anim;
using namespace render;
using namespace std::literals;

Sequence<int> parse_tile_animation(const pugi::xml_node& node) {
  std::vector<Frame<int>> frames;
  for (auto frame : node.children("frame")) {
    auto id = frame.attribute("tileid").as_int();
    auto duration = frame.attribute("duration").as_double(500.0) / 1000.0;
    frames.push_back({id, duration});
  }
  return Sequence<int>{std::move(frames), PlayMode::Loop};
}

auto parse_anim_tiles(const pugi::xml_node& node) {
  ska::flat_hash_map<int, Sequence<int>> anims;
  for (auto tile : node.children("tile")) {
    auto id = tile.attribute("id").as_int();
    if (auto anim = tile.child("animation")) {
      auto sequence = parse_tile_animation(anim);
      anims.emplace(id, sequence);
    }
  }
  return anims;
}

auto parse_collisions(const pugi::xml_node& node, const fs::path& parent_path,
                      const ssm::ivec2& size) {
  auto coll_data_path =
      node.child("properties")
          .find_child([](const auto& n) {
            return n.attribute("name").value() == "collision_data"s;
          })
          .attribute("value")
          .value();
  if (!coll_data_path) {
    return TileSet::CollisionData{ssm::ivec2(1), Matrix<bool>(size)};
  }

  auto path = parent_path / coll_data_path;

  pugi::xml_document doc;
  pugi::xml_parse_result result = doc.load_file(path.c_str());
  if (!result) {
    ERROR_LOG("Error parsing collision data file \"{}\": {}", coll_data_path,
              result.description());
    return TileSet::CollisionData{ssm::ivec2(1), Matrix<bool>(size)};
  }

  auto collision = doc.child("collision");
  auto subcols = collision.attribute("subcolumns").as_int();
  auto subrows = collision.attribute("subrows").as_int();
  auto subtile_res = ssm::ivec2(subcols, subrows);
  auto subtile_dims = size * subtile_res;
  auto matrix = Matrix<bool>(subtile_dims);

  auto coll_list = util::parse_csv(collision.child("data").child_value());
  if (coll_list.size() != matrix.num_elements()) {
    WARN_LOG("Collision data length doesn't match subtile count!");
  }
  auto n = std::min(coll_list.size(), matrix.num_elements());
  for (std::size_t i = 0; i < n; ++i) {
    matrix[i] = coll_list[i];
  }
  return TileSet::CollisionData{subtile_res, std::move(matrix)};
}

template <>
TileSet load_resource<TileSet>(const fs::path& path) {
  SCOPE_LOG("Loading tileset file \"{}\"", path.string());

  pugi::xml_document doc;
  pugi::xml_parse_result result = doc.load_file(path.c_str());
  if (!result) {
    ERROR_LOG("Error parsing file \"{}\": {}", path.string(),
              result.description());
    return TileSet{};
  }

  auto tileset = doc.child("tileset");
  std::string texture = tileset.child("image").attribute("source").value();
  texture = (path.parent_path() / texture).string();

  auto tilewidth = tileset.attribute("tilewidth").as_int();
  auto tileheight = tileset.attribute("tileheight").as_int();
  auto tilecount = tileset.attribute("tilecount").as_int();
  auto columns = tileset.attribute("columns").as_int();
  auto rows = tilecount / columns;

  auto size = ssm::ivec2(columns, rows);
  auto tile_size = ssm::ivec2(tilewidth, tileheight);

  return TileSet{texture, size, tile_size, parse_anim_tiles(tileset),
                 parse_collisions(tileset, path.parent_path(), size)};
}

template TileSet load_resource<TileSet>(const fs::path& path);
