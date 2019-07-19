#include <GLFW/glfw3.h>

#include "core/script/script.hpp"
#include "ecs/behavior.hpp"

#include "common/logging.hpp"

#include "input.hpp"

static const std::unordered_map<std::string, int> key_names = {
    {"0", GLFW_KEY_0},         {"1", GLFW_KEY_1},
    {"2", GLFW_KEY_2},         {"3", GLFW_KEY_3},
    {"4", GLFW_KEY_4},         {"5", GLFW_KEY_5},
    {"6", GLFW_KEY_6},         {"7", GLFW_KEY_7},
    {"8", GLFW_KEY_8},         {"9", GLFW_KEY_9},
    {"A", GLFW_KEY_A},         {"B", GLFW_KEY_B},
    {"C", GLFW_KEY_C},         {"D", GLFW_KEY_D},
    {"E", GLFW_KEY_E},         {"F", GLFW_KEY_F},
    {"G", GLFW_KEY_G},         {"H", GLFW_KEY_H},
    {"I", GLFW_KEY_I},         {"J", GLFW_KEY_J},
    {"K", GLFW_KEY_K},         {"L", GLFW_KEY_L},
    {"M", GLFW_KEY_M},         {"N", GLFW_KEY_N},
    {"O", GLFW_KEY_O},         {"P", GLFW_KEY_P},
    {"Q", GLFW_KEY_Q},         {"R", GLFW_KEY_R},
    {"S", GLFW_KEY_S},         {"T", GLFW_KEY_T},
    {"U", GLFW_KEY_U},         {"V", GLFW_KEY_V},
    {"W", GLFW_KEY_W},         {"X", GLFW_KEY_X},
    {"Y", GLFW_KEY_Y},         {"Z", GLFW_KEY_Z},
    {"Enter", GLFW_KEY_ENTER}, {"Backspace", GLFW_KEY_BACKSPACE},
    {"Up", GLFW_KEY_UP},       {"Down", GLFW_KEY_DOWN},
    {"Left", GLFW_KEY_LEFT},   {"Right", GLFW_KEY_RIGHT},
    {"F1", GLFW_KEY_F1},       {"F2", GLFW_KEY_F2},
    {"F3", GLFW_KEY_F3},       {"F4", GLFW_KEY_F4},
    {"F5", GLFW_KEY_F5},       {"F6", GLFW_KEY_F6},
    {"F7", GLFW_KEY_F7},       {"F8", GLFW_KEY_F8},
    {"F9", GLFW_KEY_F9},       {"F10", GLFW_KEY_F10},
    {"F11", GLFW_KEY_F11},     {"F12", GLFW_KEY_F12},
    {"F13", GLFW_KEY_F13},     {"F14", GLFW_KEY_F14},
    {"F15", GLFW_KEY_F15},     {"F16", GLFW_KEY_F16},
    {"F17", GLFW_KEY_F17},     {"F18", GLFW_KEY_F18},
    {"F19", GLFW_KEY_F19},     {"F20", GLFW_KEY_F20},
    {"F21", GLFW_KEY_F21},     {"F22", GLFW_KEY_F22},
    {"F23", GLFW_KEY_F23},     {"F24", GLFW_KEY_F24},
    {"F25", GLFW_KEY_F25},
};

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
