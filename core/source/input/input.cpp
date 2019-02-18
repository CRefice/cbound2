#include <GLFW/glfw3.h>

#include "input/context.hpp"
#include "input/input.hpp"
#include "input/key.hpp"

static void key_callback(GLFWwindow *wnd, int key, int, int action, int) {
  using input::KeyEvent;
  auto &stack = input::get_context(wnd);
  if (action == GLFW_REPEAT)
    return;
  KeyEvent event{key, action == GLFW_PRESS ? KeyEvent::Action::Press
                                           : KeyEvent::Action::Release};
  for (auto &ctx : stack) {
    if (auto it = ctx.actions.find(event); it != ctx.actions.end()) {
      auto &callback = it->second;
      callback();
      return;
    }
  }
}

namespace input {
void init(render::Context context) {
  glfwSetWindowUserPointer(context, new ContextStack());
  glfwSetKeyCallback(context, key_callback);
}

ContextStack &get_context(render::Context context) {
  return *reinterpret_cast<ContextStack *>(glfwGetWindowUserPointer(context));
}

void shutdown(render::Context context) {
  delete reinterpret_cast<ContextStack *>(glfwGetWindowUserPointer(context));
}
} // namespace input
