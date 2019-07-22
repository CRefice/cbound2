#include "core/script/script.hpp"

#include "common/logging.hpp"

#include "input.hpp"

namespace fw {
using namespace ecs;
using Traits = LuaTraits<ActionMap>;

std::optional<ActionMap> Traits::parse(const sol::table& tbl) {
  ActionMap actions;
  for (const auto& iter : tbl) {
    auto key = iter.first.as<std::string>();
    auto fn = iter.second.as<sol::function>();
    actions.emplace(key, fn);
  }
  return actions;
}
} // namespace fw
