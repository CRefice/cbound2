#include <cassert>

#include <GLFW/glfw3.h>

#include "input/keyboard.hpp"

namespace input {
static void set_handler(GLFWwindow* context, KeyboardHandler* handler) {
  glfwSetWindowUserPointer(context, handler);
}

static KeyboardHandler* get_handler(GLFWwindow* context) {
  return static_cast<KeyboardHandler*>(glfwGetWindowUserPointer(context));
}

static void key_callback(GLFWwindow* wnd, int key, int, int action, int) {
  auto* handler = get_handler(wnd);
  if (action == GLFW_REPEAT)
    return;
  handler->handle(key, action == GLFW_PRESS);
}

KeyboardHandler::KeyboardHandler(GLFWwindow* ctx, ActionQueue& queue,
                                 ButtonMap mapping)
    : ctx(ctx), queue(queue), mapping(std::move(mapping)) {
  assert(get_handler(ctx) == nullptr);
  set_handler(ctx, this);
  glfwSetKeyCallback(ctx, key_callback);
}

KeyboardHandler::~KeyboardHandler() { set_handler(ctx, nullptr); };

void KeyboardHandler::handle(int keycode, bool pressed) {
  if (auto mapper = mapping.find(keycode); mapper != mapping.end()) {
    auto action = mapper->second(pressed);
    queue.push(std::move(action));
  }
}
} // namespace input
