#pragma once

#include <hash/flat_hash_map.hpp>
#include <sol/sol.hpp>

#include "core/script/scheduler.hpp"

#include "entity.hpp"

namespace ecs {
using BehaviorScheduler = script::CoroutineScheduler<EntityId>;
using BehaviorRunner = script::SchedulerValueAdapter<EntityId, sol::table>;

// Calls an update function each frame on each registered entity.
class BehaviorManager {
public:
  void submit(EntityId id, sol::function fn) {
    behaviors.insert_or_assign(id, std::move(fn));
  }
  void remove(const EntityId& id) { behaviors.erase(id); }

  void update(double dt, BehaviorRunner& runner);

	void load_entity(const EntityId& id, sol::table& table);

private:
  ska::flat_hash_map<EntityId, sol::function> behaviors;
};
} // namespace ecs
