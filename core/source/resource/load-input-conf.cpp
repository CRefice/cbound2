#include <pugixml.hpp>

#include "common/logging.hpp"

#include "input/input.hpp"
#include "resource/resource.hpp"

namespace fs = std::filesystem;
using namespace input;

using NameMap = std::unordered_map<std::string, int>;

static const NameMap keeb_btn_names = {
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

static const NameMap gamepad_btn_names = {
    {"A", GLFW_GAMEPAD_BUTTON_A},
    {"B", GLFW_GAMEPAD_BUTTON_B},
    {"X", GLFW_GAMEPAD_BUTTON_X},
    {"Y", GLFW_GAMEPAD_BUTTON_Y},
    {"LB", GLFW_GAMEPAD_BUTTON_LEFT_BUMPER},
    {"RB", GLFW_GAMEPAD_BUTTON_RIGHT_BUMPER},
    {"LT", GLFW_GAMEPAD_BUTTON_LEFT_THUMB},
    {"RT", GLFW_GAMEPAD_BUTTON_RIGHT_THUMB},
    {"Start", GLFW_GAMEPAD_BUTTON_START},
    {"Back", GLFW_GAMEPAD_BUTTON_BACK},
    {"DpadU", GLFW_GAMEPAD_BUTTON_DPAD_UP},
    {"DpadD", GLFW_GAMEPAD_BUTTON_DPAD_DOWN},
    {"DpadL", GLFW_GAMEPAD_BUTTON_DPAD_LEFT},
    {"DpadR", GLFW_GAMEPAD_BUTTON_DPAD_RIGHT},
};

static const NameMap gamepad_axis_names = {
    {"LX", GLFW_GAMEPAD_AXIS_LEFT_X},
    {"LY", GLFW_GAMEPAD_AXIS_LEFT_Y},
    {"RX", GLFW_GAMEPAD_AXIS_RIGHT_X},
    {"RY", GLFW_GAMEPAD_AXIS_RIGHT_Y},
    {"LT", GLFW_GAMEPAD_AXIS_LEFT_TRIGGER},
    {"RT", GLFW_GAMEPAD_AXIS_RIGHT_TRIGGER},
};

AxisMap parse_axis_map(const pugi::xml_node& node, const NameMap& names) {
  AxisMap map;
  for (auto axis : node.children("axis")) {
    auto name = axis.attribute("name").value();
    auto action = axis.attribute("action").value();
    if (auto it = names.find(name); it != names.end()) {
      auto key = it->second;
      if (axis.attribute("invert").as_bool()) {
        map.emplace(key, [action](double val) { return Action{action, -val}; });
      } else {
        map.emplace(key, [action](double val) { return Action{action, val}; });
      }
    } else {
      WARN_LOG("'{}' does not corrispond to a device axis!", name);
    }
  }
  return map;
}

ButtonMap parse_button_map(const pugi::xml_node& node, const NameMap& names) {
  ButtonMap map;
  for (auto button : node.children("button")) {
    auto name = button.attribute("name").value();
		std::string action = button.attribute("action").value();
    if (auto it = names.find(name); it != names.end()) {
      auto key = it->second;
      map.emplace(key, [action](double val) { return Action{action, val}; });
    } else {
      WARN_LOG("'{}' does not correspond to a device button!", name);
    }
  }
  for (auto axis : node.children("button_axis")) {
    auto plus = axis.attribute("positive").value();
    auto minus = axis.attribute("negative").value();
		std::string action = axis.attribute("action").value();
    if (auto it1 = names.find(plus), it2 = names.find(minus);
        it1 != names.end() && it2 != names.end()) {
      auto key1 = it1->second, key2 = it2->second;
      map.emplace(key1, [action](double val) { return Action{action, val}; });
      map.emplace(key2, [action](double val) { return Action{action, -val}; });
    } else {
      if (it1 == names.end()) {
        WARN_LOG("'{}' does not corrispond to a device button!", plus);
      }
      if (it2 == names.end()) {
        WARN_LOG("'{}' does not corrispond to a device button!", minus);
      }
    }
  }
  return map;
}

GamepadConfig parse_gamepad_config(const pugi::xml_node& node) {
  ButtonMap buttons = parse_button_map(node, gamepad_btn_names);
  AxisMap axes = parse_axis_map(node, gamepad_axis_names);
  double dead_zone = node.child("dead_zone").attribute("value").as_double();
  return GamepadConfig{buttons, axes, dead_zone};
}

template <>
InputConfig load_resource<InputConfig>(const fs::path& path) {
  SCOPE_LOG("Loading input configuration file \"{}\"", path.string());

  pugi::xml_document doc;
  pugi::xml_parse_result result = doc.load_file(path.c_str());
  if (!result) {
    ERROR_LOG("Error parsing file \"{}\": {}", path.string(),
              result.description());
    return InputConfig{};
  }

  auto input = doc.child("input");
  auto keyboard = parse_button_map(input.child("keyboard"), keeb_btn_names);
  auto gamepad = parse_gamepad_config(input.child("gamepad"));
  return InputConfig{keyboard, gamepad};
}

template InputConfig load_resource<InputConfig>(const fs::path& path);
