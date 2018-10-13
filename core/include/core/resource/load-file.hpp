#pragma once

#include <filesystem>

#include "core/render/font.hpp"
#include "core/render/texture.hpp"
#include "core/render/shader.hpp"

render::Font load_font(const std::filesystem::path& file);
shader::Stage load_shader(const std::filesystem::path& file);
render::Texture load_texture(const std::filesystem::path& path);
