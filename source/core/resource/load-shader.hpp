#pragma once

#include <string>
#include <utility>

#include "core\rendering\shader.hpp"

Shader load_shader(const std::pair<std::string, std::string> paths);
