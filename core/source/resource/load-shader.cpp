#include <cstdlib>
#include <fstream>

#include "common/logging.hpp"
#include "render/shader.hpp"

#include "resource/resource.hpp"

namespace fs = std::filesystem;

template <>
shader::Stage load_resource<shader::Stage>(const fs::path& path) {
  SCOPE_LOG("Loading shader file \"{}\"", path.string());
  shader::Type type;
  if (path.string().rfind(".f") != std::string::npos)
    type = shader::Type::Fragment;
  else
    type = shader::Type::Vertex;

  std::ifstream file(path, std::ios::ate);
  if (!file.good()) {
    ERROR_LOG("Unable to open file \"{}\"", path.string());
		return shader::Stage(type, "");
  }
  auto length = file.tellg();
  file.seekg(0, std::ios::beg);
  char* source = new char[length];
  file.read(source, length);

  shader::Stage stage(type, std::string_view(source, length));
  delete[] source;
  return stage;
}

template shader::Stage load_resource<shader::Stage>(const fs::path& path);
