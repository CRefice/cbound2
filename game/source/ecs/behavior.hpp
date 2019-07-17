#pragma once

#include <hash/flat_hash_map.hpp>
#include <sol/sol.hpp>

#include "common/logging.hpp"
#include "core/script/script.hpp"

#include "entity.hpp"

namespace ecs {
struct Closure {
  sol::table tbl;
  sol::function fn;
};

class BehaviorManager {
public:
  template <typename... Args>
  void run(EntityId id, Closure call, Args&&... args) {
    if (auto previous = coros.find(id); previous != coros.end()) {
      return;
    }
    auto thread_it = threads.find(id);
    if (thread_it == threads.end()) {
      thread_it =
          threads.emplace(id, sol::thread::create(call.fn.lua_state())).first;
    }
    sol::state_view runner_state = thread_it->second.state();
    sol::coroutine coro(runner_state, call.fn);
    auto result = coro(call.tbl, std::forward<Args>(args)...);
    if (!result.valid()) {
      sol::error err = result;
      ERROR_LOG("Lua error: {}", err.what());
			threads.erase(id);
    } else if (coro.runnable()) {
      coros.emplace(id, coro);
    }
  }

  void cancel(EntityId id);

  void tick_all();

private:
  ska::flat_hash_map<EntityId, sol::coroutine> coros;
  ska::flat_hash_map<EntityId, sol::thread> threads;
};

// Calls an update function on each submitted entity, every frame
class UpdateManager {
public:
  void submit(EntityId id, Closure);
  void remove(EntityId id);

  void tick_all(BehaviorManager& behav);

private:
  ska::flat_hash_map<EntityId, Closure> updates;
};
} // namespace ecs
