#pragma once

#include <optional>

#include <sol/sol.hpp>

#include "core/render/tiles.hpp"
#include "core/render/tex-coords.hpp"

namespace fw::tiles {
// Add all the framework definitions to the given state.
void load_libraries(sol::state& state);

// Parse a tile set from the given table
std::optional<::render::TileSet> parse_tileset(const sol::table& table);

// Parse a tile map from the given table
std::optional<::render::TileMap> parse_tilemap(const sol::table& table);
} // namespace fw::tiles
