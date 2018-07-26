#pragma once

#include <filesystem>

#include "../render/shader.hpp"

shader::Stage load_shader(const std::filesystem::path& file);
