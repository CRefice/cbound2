#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "action.hpp"

namespace input {
struct GamepadConfig {
  ButtonMap buttons;
  AxisMap axes;
  double dead_zone = 0.01;
};

class Gamepad {
public:
  Gamepad(int id, ActionQueue& queue, GamepadConfig config)
      : id(id), queue(queue), config(std::move(config)) {}

  std::string name() const;
  void poll();

private:
	bool is_dead(double val);

  void handle_button(int btn, bool pressed);
  void handle_axis(int axs, double value);

  int id;
  GLFWgamepadstate state{};

  ActionQueue& queue;
  GamepadConfig config;
};
} // namespace input
