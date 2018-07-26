#pragma once

#include <filesystem>

#include "render/texture.hpp"

Texture load_texture(const std::filesystem::path& path);
