#pragma once

#include <vector>

#include <hash/flat_hash_map.hpp>

#include "core/input/action.hpp"

#include "behavior.hpp"
#include "entity.hpp"

namespace ecs {
using ActionCallback = Closure;

struct InputContext {
  bool handle(const EntityId& id, const input::Action& action,
              BehaviorManager& behav);

  ska::flat_hash_map<std::string, ActionCallback> actions;
};

class InputManager {
public:
  InputManager(input::ActionQueue& queue) : queue(queue) {}

  void submit(const EntityId& id, InputContext ctx);
  void remove(const EntityId& id);

  void dispatch(BehaviorManager& behav);

private:
  input::ActionQueue& queue;
  std::vector<std::pair<EntityId, InputContext>> contexts;
};
} // namespace ecs
