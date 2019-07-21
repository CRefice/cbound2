#pragma once

#include "gamepad.hpp"
#include "keyboard.hpp"

namespace input {
struct InputConfig {
  ButtonMap keyboard_map;
  GamepadConfig pad_conf;
};

// Utility wrapper for keyboard and gamepad input functionality
class InputManager {
public:
  InputManager(GLFWwindow* wnd, ActionQueue& queue, InputConfig config)
      : keeb(wnd, queue, std::move(config.keyboard_map)),
        gamepad(GLFW_JOYSTICK_1, queue, std::move(config.pad_conf)) {}

  void poll() { gamepad.poll(); }

private:
  KeyboardHandler keeb;
  Gamepad gamepad;
};
} // namespace input
