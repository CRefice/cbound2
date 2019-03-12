#include <cctype>
#include <cstdlib>
#include <fstream>
#include <unordered_map>

#include "common/logging.hpp"
#include "render/font.hpp"

#include "resource/resource.hpp"

namespace fs = std::filesystem;
using namespace render;

static auto parse_frames(std::istream& s) {
  std::unordered_map<std::string, CharMetrics> ret;
  for (std::string line; std::getline(s, line);) {
    std::istringstream ss(line);
    std::string key;
    if (std::isspace(line[0])) {
      key = line.substr(0, 1);
    } else {
      ss >> key;
    }
    int x1, y1, x2, y2, dip = 0;
    ss >> x1 >> y1 >> x2 >> y2 >> dip;
    TexFrame rect({x1, y1}, {x2, y2});
    ret.insert_or_assign(key, CharMetrics{rect, dip});
  }
  return ret;
}

template <>
Font load_resource<Font>(const fs::path& path) {
  SCOPE_LOG("Loading font: {}", path.string());
  std::ifstream file(path);
  if (!file.good()) {
    ERROR_LOG("Unable to open file {}", path.string());
  }
  std::string texture_id;
  if (!std::getline(file, texture_id)) {
    ERROR_LOG("File {} is empty", path.string());
  }
  auto map = parse_frames(file);
  return Font{texture_id, std::move(map)};
}

template Font load_resource<Font>(const fs::path& path);
