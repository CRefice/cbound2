#pragma once

#include <vector>

#include <hash/flat_hash_map.hpp>

#include "core/input/action.hpp"

#include "behavior.hpp"
#include "entity.hpp"

namespace ecs {
using ActionMap = ska::flat_hash_map<std::string, sol::function>;

class InputManager {
public:
  InputManager(input::ActionQueue& queue) : queue(queue) {}

  void submit(const EntityId& id, ActionMap actions);
  void remove(const EntityId& id);

  void dispatch(BehaviorRunner& runner);

	void load_entity(const EntityId& id, sol::table& entity);

private:
  input::ActionQueue& queue;
  std::vector<std::pair<EntityId, ActionMap>> entity_actions;
};
} // namespace ecs
