#pragma once

#include <filesystem>

#include "core/render/tiles.hpp"

namespace editor::io {
void save_collision(const std::filesystem::path& path, const ::render::TileSet& set);
}
