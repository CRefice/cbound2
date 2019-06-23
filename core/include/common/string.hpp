#pragma once

#include <string_view>
#include <vector>

namespace util {
std::vector<int> parse_csv(std::string_view s);
}
