#pragma once

#include <hash/flat_hash_map.hpp>
#include <sol/sol.hpp>

#include "entity.hpp"

namespace ecs {
class BehaviorManager {
public:
  void run(EntityId id, sol::function fn);
  void cancel(EntityId id);

  void tick_all();

private:
  ska::flat_hash_map<EntityId, sol::coroutine> coros;
  ska::flat_hash_map<EntityId, sol::thread> threads;
};
} // namespace ecs
