#pragma once

#include <functional>
#include <queue>

#include <hash/flat_hash_map.hpp>

namespace input {
struct Action {
  std::string name;
  double state;
};

using ActionQueue = std::queue<Action>;

using AxisMap = ska::flat_hash_map<int, std::function<Action(double)>>;
using ButtonMap = ska::flat_hash_map<int, std::function<Action(bool)>>;
} // namespace input
