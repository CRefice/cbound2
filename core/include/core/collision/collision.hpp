#pragma once

#include <algorithm>
#include <optional>

#include "common/rectangle.hpp"

namespace collision {

template <typename T>
bool overlap(const Rectangle<T>& a, const Rectangle<T>& b) {
  return a.left() < b.right() && a.right() > b.left() && a.bottom() < b.top() &&
         a.top() > b.bottom();
}

template <typename T>
std::optional<Rectangle<T>> intersection(const Rectangle<T>& a,
                                         const Rectangle<T>& b) {
  auto start = ssm::vector<T, 2>(std::max(a.left(), b.left()),
                                 std::max(a.bottom(), b.bottom()));
  auto end = ssm::vector<T, 2>(std::min(a.right(), b.right()),
                               std::min(a.top(), b.top()));

  if (start.x > end.x || start.y > end.y) {
    return std::nullopt;
  }

  return Rectangle<T>(start, end);
}
} // namespace collision
