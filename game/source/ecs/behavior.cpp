#include "behavior.hpp"

namespace ecs {
void BehaviorManager::run(EntityId id, sol::function fn) {
  if (auto previous = coros.find(id); previous != coros.end()) {
    return;
  }
  auto thread_it = threads.find(id);
  if (thread_it == threads.end()) {
    thread_it = threads.emplace(id, sol::thread::create(fn.lua_state())).first;
  }
  sol::state_view runner_state = thread_it->second.state();
  sol::coroutine coro(runner_state, fn);
  coro(std::move(id));
  if (coro.runnable()) {
    coros.emplace(id, coro);
  }
}

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
