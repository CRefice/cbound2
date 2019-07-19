#include "input/gamepad.hpp"

namespace input {
std::string Gamepad::name() const { return glfwGetGamepadName(id); }

void Gamepad::poll() {
  GLFWgamepadstate new_state;
  if (!glfwGetGamepadState(id, &new_state)) {
    return;
  }
  for (int i = 0; i < GLFW_GAMEPAD_BUTTON_LAST; ++i) {
    if (new_state.buttons[i] != state.buttons[i]) {
      handle_button(i, new_state.buttons[i] == GLFW_PRESS);
    }
  }
  for (int i = 0; i < GLFW_GAMEPAD_AXIS_LAST; ++i) {
    if (new_state.axes[i] != state.axes[i]) {
      handle_axis(i, new_state.axes[i]);
    }
  }
  state = std::move(new_state);
}

void Gamepad::handle_button(int btn, bool pressed) {
  if (auto mapping = buttons.find(btn); mapping != buttons.end()) {
    auto action = mapping->second(pressed);
    queue.push(action);
  }
}

void Gamepad::handle_axis(int axs, double value) {
  if (auto mapping = axes.find(axs); mapping != axes.end()) {
    auto action = mapping->second(value);
    queue.push(action);
  }
}
} // namespace input
