#include <algorithm>

#include "input.hpp"

using namespace input;

namespace ecs {
bool InputContext::handle(const EntityId& id, const Action& action,
                          BehaviorManager& behav) {
  auto it = actions.find(action.name);
  if (it != actions.end()) {
    behav.run(id, it->second, action.state);
    return true;
  }
  return false;
}

void InputManager::submit(const EntityId& id, InputContext ctx) {
  contexts.emplace_back(id, std::move(ctx));
}

void InputManager::remove(const EntityId& id) {
  for (auto it = contexts.rbegin(); it != contexts.rend(); ++it) {
    if (it->first == id) {
      contexts.erase(std::next(it).base());
    }
  }
}

void InputManager::dispatch(BehaviorManager& behav) {
  while (!queue.empty()) {
    const auto& action = queue.front();

    for (auto it = contexts.rbegin(); it != contexts.rend(); ++it) {
      const auto& id = it->first;
      auto& ctx = it->second;
      if (ctx.handle(id, action, behav)) {
        break;
      }
    }
    queue.pop();
  }
}
} // namespace ecs
