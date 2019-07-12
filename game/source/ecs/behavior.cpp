#include "behavior.hpp"

namespace ecs {
void BehaviorManager::cancel(EntityId id) { coros.erase(id); }

void BehaviorManager::tick_all() {
  for (auto it = coros.begin(); it != coros.end();) {
    auto& coro = it->second;
    if (!coro.runnable()) {
      it = coros.erase(it);
    } else {
      coro();
      ++it;
    }
  }
}

void UpdateManager::submit(EntityId id, sol::function fn) {
  updates.emplace(id, fn);
}

void UpdateManager::remove(EntityId id) {
  updates.erase(id);
}

void UpdateManager::tick_all(BehaviorManager& behav) {
  for (auto [id, fn] : updates) {
		behav.run(id, fn);
  }
}
} // namespace ecs
