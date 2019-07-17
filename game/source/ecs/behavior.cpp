#include "behavior.hpp"

namespace ecs {
void BehaviorManager::cancel(EntityId id) { coros.erase(id); }

void BehaviorManager::tick_all() {
  for (auto it = coros.begin(); it != coros.end();) {
    auto& coro = it->second;
    if (!coro.runnable()) {
      it = coros.erase(it);
    } else {
      auto result = coro();
      if (!result.valid()) {
        sol::error err = result;
        ERROR_LOG("Lua error: {}", err.what());
        it = coros.erase(it);
				threads.erase(it->first);
      } else {
        ++it;
      }
    }
  }
}

void UpdateManager::submit(EntityId id, Closure call) {
  updates.emplace(id, std::move(call));
}

void UpdateManager::remove(EntityId id) { updates.erase(id); }

void UpdateManager::tick_all(BehaviorManager& behav) {
  for (auto& elem : updates) {
    const auto& id = elem.first;
    const auto& call = elem.second;
    behav.run(id, call);
  }
}
} // namespace ecs
