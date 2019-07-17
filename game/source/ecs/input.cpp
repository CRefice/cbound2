#include <algorithm>

#include <sol/sol.hpp>

#include "input.hpp"

namespace ecs {
bool InputContext::handle(EntityId id, input::KeyEvent event,
                          BehaviorManager &behav) {
  auto it = actions.find(event);
  if (it != actions.end()) {
    behav.run(id, it->second, id);
    return true;
  }
  return false;
}

void InputManager::handle(input::KeyEvent event) {
  for (auto it = contexts.rbegin(); it != contexts.rend(); ++it) {
    auto &[id, ctx] = *it;
    if (ctx.handle(id, event, behave))
      return;
  }
}

void InputManager::remove(EntityId id) {
  for (auto it = contexts.rbegin(); it != contexts.rend(); ++it) {
    const auto &[entry_id, ctx] = *it;
    if (id == entry_id) {
      std::swap(*it, contexts.back());
      to_delete++;
      return;
    }
  }
}

void InputManager::delete_dead() {
  while (to_delete) {
    contexts.pop_back();
    to_delete--;
  }
}
} // namespace ecs
