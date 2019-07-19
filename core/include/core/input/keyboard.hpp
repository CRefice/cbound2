#pragma once

#include "action.hpp"

class GLFWwindow;

namespace input {
class KeyboardHandler {
public:
  KeyboardHandler(GLFWwindow* ctx, ActionQueue& queue, ButtonMap mapping);
  ~KeyboardHandler();

  KeyboardHandler(KeyboardHandler&&) = default;
  KeyboardHandler& operator=(KeyboardHandler&&) = delete;
  KeyboardHandler(const KeyboardHandler&) = delete;
  KeyboardHandler& operator=(const KeyboardHandler&) = delete;

  void handle(int keycode, bool pressed);

private:
  GLFWwindow* ctx;
  ActionQueue& queue;
  ButtonMap mapping;
};
} // namespace input
