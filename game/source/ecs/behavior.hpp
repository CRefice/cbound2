#pragma once

#include <hash/flat_hash_map.hpp>
#include <sol/sol.hpp>

#include "entity.hpp"

namespace ecs {
class BehaviorManager {
public:
  template <typename... Args>
  void run(EntityId id, sol::function fn, Args&&... args) {
    if (auto previous = coros.find(id); previous != coros.end()) {
      return;
    }
    auto thread_it = threads.find(id);
    if (thread_it == threads.end()) {
      thread_it =
          threads.emplace(id, sol::thread::create(fn.lua_state())).first;
    }
    sol::state_view runner_state = thread_it->second.state();
    sol::coroutine coro(runner_state, fn);
    coro(std::move(id), std::forward<Args>(args)...);
    if (coro.runnable()) {
      coros.emplace(id, coro);
    }
  }

  void cancel(EntityId id);

  void tick_all();

private:
  ska::flat_hash_map<EntityId, sol::coroutine> coros;
  ska::flat_hash_map<EntityId, sol::thread> threads;
};
} // namespace ecs
