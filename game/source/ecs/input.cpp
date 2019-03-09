#include "input.hpp"

namespace ecs {
bool InputContext::handle(EntityId id, input::KeyEvent event) {
  auto it = actions.find(event);
  if (it != actions.end()) {
    auto &fn = it->second;
    fn(id);
    return true;
  }
  return false;
}

void InputManager::handle(input::KeyEvent event) {
  for (auto it = contexts.rbegin(); it != contexts.rend(); ++it) {
    auto &[id, ctx] = *it;
    if (ctx.handle(id, event))
      return;
  }
}
} // namespace ecs
