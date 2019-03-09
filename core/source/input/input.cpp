#include <GLFW/glfw3.h>

#include "input/key.hpp"
#include "input/input.hpp"

static void key_callback(GLFWwindow* wnd, int key, int, int action, int) {
  using input::KeyEvent;
  auto& handler = input::get_handler(wnd);
  if (action == GLFW_REPEAT)
    return;
  KeyEvent event{key, action == GLFW_PRESS ? KeyEvent::Action::Press
                                           : KeyEvent::Action::Release};
  handler.handle(event);
}

namespace input {
void init(render::Context context) {
  glfwSetKeyCallback(context, key_callback);
}

void set_handler(render::Context context, KeyHandler& handler) {
  glfwSetWindowUserPointer(context, &handler);
}

KeyHandler& get_handler(render::Context context) {
  return *reinterpret_cast<KeyHandler*>(glfwGetWindowUserPointer(context));
}

void shutdown(render::Context context) {}
} // namespace input
