#include <vector>

#include <sol/forward.hpp>

#include "common/logging.hpp"

#include "core/script/script.hpp"

#include "tiles.hpp"

namespace fw::tiles {
using namespace ::anim;
using ::render::TileSet;
using ::render::TileMap;

void load_libraries(sol::state& state) { }

std::vector<Frame<int>> parse_frames(const sol::table& table) {
	std::vector<Frame<int>> frames;
	frames.reserve(table.size());
	for (const auto& [key, val] : table) {
		const sol::table& tbl = val;
		double dur = tbl.get_or("duration", 0.016); //Default duration of one frame
		if (auto id = tbl.get<sol::optional<int>>("id")) {
			frames.push_back({*id, dur});
		} else {
			WARN_LOG("Animation frame data contains non-tile data");
		}
	}
	return frames;
}

std::unordered_map<int, Sequence<int>> parse_tiles(const sol::table& table) {
	std::unordered_map<int, Sequence<int>> tiles;
	tiles.reserve(table.size());
	for (const auto& [key, val] : table) {
		const sol::table& tbl = val;
		auto id = tbl.get<sol::optional<int>>("id");
		auto frames = tbl.get<sol::optional<sol::table>>("frames");
		if (id && frames) {
			tiles.emplace(*id, Sequence<int>{ parse_frames(*frames), PlayMode::Loop });
		} else {
			WARN_LOG("Tileset data contains non-tile data");
		}
	}
	return tiles;
}

std::optional<TileSet> parse_tileset(const sol::table& table) {
	std::string image = table["image"];
	auto size = script::parse_vec2<int>(table["size"]);
	auto tile_size = script::parse_vec2<int>(table["tile_size"]);
	if (size && tile_size) {
		auto tiles = parse_tiles(table["tiles"]);
		return TileSet{ std::move(image), *size, *tile_size, std::move(tiles) };
	}
	return std::nullopt;
}

TileMap::Layer parse_layer(const sol::table& table, int default_depth) {
	int depth = table.get_or("depth", default_depth);
	const sol::table& tile_tbl = table["tiles"];
	std::vector<int> tiles;
	tiles.reserve(tile_tbl.size());
	for (std::size_t i = 1; i <= tile_tbl.size(); ++i) {
		if (sol::optional<int> maybe_tile = tile_tbl[i]) {
			tiles.emplace_back(*maybe_tile);
		} else {
			WARN_LOG("Tilemap data contains non-tile data");
		}
	}
	return TileMap::Layer{depth, tiles};
}

std::vector<TileMap::Layer> parse_layers(const sol::table& table) {
	std::vector<TileMap::Layer> layers;
	layers.reserve(table.size());
	int depth = table.size() + 1;
	for (const auto& [key, val] : table) {
		layers.emplace_back(parse_layer(val, depth--));
	}
	return layers;
}

// Parse a tile map from the given table
std::optional<::render::TileMap> parse_tilemap(const sol::table& table) {
	auto size = script::parse_vec2<int>(table["size"]);
	auto tile_size = script::parse_vec2<float>(table["tile_size"]);
	if (size && tile_size) {
		auto layers = parse_layers(table["layers"]);
		return TileMap{*size, *tile_size, layers};
	}
	return std::nullopt;
}
} // namespace fw::anim
