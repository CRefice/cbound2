#pragma once

#include <cstdint>
#include <functional>
#include <utility>

struct ordered_pair_hash {
  template <typename T1, typename T2>
  std::size_t operator()(const std::pair<T1, T2>& pair) const {
    auto a = std::hash<T1>()(pair.first);
    auto b = std::hash<T2>()(pair.second);
    return a >= b ? a * a + a + b : a + b * b;
  }
};
