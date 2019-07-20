#include <cmath>

#include "input/gamepad.hpp"

namespace input {
std::string Gamepad::name() const { return glfwGetGamepadName(id); }

void Gamepad::poll() {
  GLFWgamepadstate new_state;
  if (!glfwGetGamepadState(id, &new_state)) {
    return;
  }
  for (int i = 0; i <= GLFW_GAMEPAD_BUTTON_LAST; ++i) {
    if (new_state.buttons[i] != state.buttons[i]) {
      handle_button(i, new_state.buttons[i] == GLFW_PRESS);
    }
  }
  for (int i = 0; i <= GLFW_GAMEPAD_AXIS_LAST; ++i) {
    auto axs = new_state.axes[i];
    auto old = state.axes[i];
    bool changed =
        axs != old &&
        (!is_dead(axs) ||                  // New state isn't in dead zone
         (is_dead(axs) && !is_dead(old))); // New state just fell into dead zone
    if (changed) {
      handle_axis(i, axs);
    }
  }
  state = new_state;
}

void Gamepad::handle_button(int btn, bool pressed) {
  if (auto mapping = config.buttons.find(btn);
      mapping != config.buttons.end()) {
    auto action = mapping->second(pressed);
    queue.push(action);
  }
}

void Gamepad::handle_axis(int axs, double value) {
  if (auto mapping = config.axes.find(axs); mapping != config.axes.end()) {
    auto action = mapping->second(value);
    if (is_dead(action.state)) {
      action.state = 0.0;
    }
    queue.push(action);
  }
}

bool Gamepad::is_dead(double val) { return std::abs(val) < config.dead_zone; }
} // namespace input
