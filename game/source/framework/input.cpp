#include <GLFW/glfw3.h>

#include "core/script/script.hpp"
#include "ecs/behavior.hpp"

#include "common/logging.hpp"

#include "input.hpp"

using namespace ecs;
using namespace ::input;

namespace fw::input {
std::optional<InputContext> parse_context(const sol::table& tbl,
                                          sol::table table) {
  InputContext ctx;
  for (auto iter : table) {
    auto key = iter.first.as<std::string>();
    auto fn = iter.second.as<sol::function>();
    if (char sign = key.back(); sign == '+' || sign == '-') {
      key = key.substr(0, key.size() - 1);
      auto function = std::function([sign, fn](sol::table tbl, double state) {
        if ((sign == '+' && state > 0.0) || (sign == '-' && state <= 0.0)) {
          fn(tbl);
        }
      });
      table[key] = function;
    }
    ctx.actions.emplace(key, Closure{tbl, table[key]});
  }
  return ctx;
}
} // namespace fw::input
