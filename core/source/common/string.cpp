#include <sstream>
#include <iostream>

#include "common/string.hpp"

namespace util {
std::vector<int> parse_csv(std::string_view s) {
  std::stringstream ss(s.data());
  std::vector<int> ret;
  while (ss.good()) {
    std::string substr;
    std::getline(ss, substr, ',');
    ret.push_back(std::stoi(substr));
  }
  return ret;
}
} // namespace util
