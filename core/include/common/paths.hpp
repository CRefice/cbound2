#pragma once

#include <filesystem>

namespace path
{
namespace fs = std::filesystem;
// Path to the currently running executable
fs::path executable_path();
// The base directory where the executable is located
fs::path install_dir();
}
