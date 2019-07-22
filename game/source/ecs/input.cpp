#include <algorithm>

#include "framework/input.hpp"

#include "input.hpp"

using namespace input;

namespace ecs {
static bool try_dispatch(const ActionMap& actions, const Action& action,
                         const EntityId& id, BehaviorRunner& runner) {
  if (auto it = actions.find(action.name); it != actions.end()) {
    runner.run(id, it->second, action.state);
    return true;
  }
  return false;
}

void InputManager::submit(const EntityId& id, ActionMap actions) {
  entity_actions.emplace_back(id, std::move(actions));
}

void InputManager::remove(const EntityId& id) {
  for (auto it = entity_actions.rbegin(); it != entity_actions.rend(); ++it) {
    if (it->first == id) {
      entity_actions.erase(std::next(it).base());
    }
  }
}

void InputManager::dispatch(BehaviorRunner& runner) {
  while (!queue.empty()) {
    const auto& action = queue.front();
    for (auto it = entity_actions.rbegin(); it != entity_actions.rend(); ++it) {
      const auto& id = it->first;
      const auto& map = it->second;
      if (try_dispatch(map, action, id, runner)) {
        break;
      }
    }
    queue.pop();
  }
}

void InputManager::load_entity(const EntityId& id, sol::table& entity) {
  if (auto table = entity["input"]) {
    if (auto input = fw::LuaTraits<ActionMap>::parse(table)) {
      submit(id, *input);
      table = sol::nil;
    } else {
      WARN_LOG("Unable to parse input component!");
    }
  }
}
} // namespace ecs
