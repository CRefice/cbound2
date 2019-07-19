#pragma once

#include <GLFW/glfw3.h>

#include "action.hpp"
#include "input.hpp"

namespace input {
class Gamepad {
public:
  Gamepad(int id, ActionQueue& queue, ButtonMap buttons, AxisMap axes)
      : id(id), queue(queue), buttons(std::move(buttons)),
        axes(std::move(axes)) {}

  std::string name() const;
  void poll();

private:
  void handle_button(int btn, bool pressed);
  void handle_axis(int axs, double value);

  int id;
  GLFWgamepadstate state{};

  ActionQueue& queue;
  ButtonMap buttons;
  AxisMap axes;
};
} // namespace input
