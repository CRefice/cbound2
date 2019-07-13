#pragma once

#include <filesystem>

#include "core/tiles/tiles.hpp"

namespace editor::io {
void save_collision(const std::filesystem::path& path, const TileSet& set);
}
