#include <sstream>

#include <pugixml.hpp>

#include "common/logging.hpp"
#include "common/string.hpp"
#include "render/tiles.hpp"

#include "resource/resource.hpp"

namespace fs = std::filesystem;
using namespace render;
using namespace std::literals;

TileMap::Layer parse_layer(const pugi::xml_node& node) {
  auto depth = node.child("properties")
                   .find_child([](const auto& n) {
                     return n.attribute("name").value() == "Depth"s;
                   })
                   .attribute("value")
                   .as_int(-1);

  auto data = util::parse_csv(node.child("data").child_value());
  return TileMap::Layer{depth, std::move(data)};
}

std::vector<TileMap::Layer> parse_layers(const pugi::xml_node& node) {
  std::vector<TileMap::Layer> layers;
  for (auto layer : node.children("layer")) {
    layers.push_back(parse_layer(layer));
  }

  // Set default depths after the fact, since we
  // don't know the number of layers in advance
  int depth = layers.size();
  for (auto& layer : layers) {
    if (layer.depth == -1) {
      layer.depth = depth;
    }
    depth--;
  }
	return layers;
}

template <>
TileMap load_resource<TileMap>(const fs::path& path) {
  SCOPE_LOG("Loading tilemap file \"{}\"", path.string());

  pugi::xml_document doc;
  pugi::xml_parse_result result = doc.load_file(path.c_str());
  if (!result) {
    ERROR_LOG("Error parsing file \"{}\": {}", path.string(),
              result.description());
    return TileMap{};
  }

  auto tilemap = doc.child("map");

  auto width = tilemap.attribute("width").as_int();
  auto height = tilemap.attribute("height").as_int();
  auto tilewidth = tilemap.attribute("tilewidth").as_float();
  auto tileheight = tilemap.attribute("tileheight").as_float();

  auto size = ssm::ivec2(width, height);
  auto tile_size = ssm::vec2(tilewidth, tileheight);

  return TileMap{size, tile_size, parse_layers(tilemap)};
}

template TileMap load_resource<TileMap>(const fs::path& path);
