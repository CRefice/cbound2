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
} // namespace ecs
