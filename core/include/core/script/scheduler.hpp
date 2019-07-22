#pragma once

#include <hash/flat_hash_map.hpp>
#include <sol/sol.hpp>

#include "common/logging.hpp"

// Associates a Key type with a coroutine, and runs all
// of them at regular intervals.
namespace script {
template <typename Key>
class CoroutineScheduler {
public:
  CoroutineScheduler(sol::state_view state) : state(std::move(state)) {}

  template <typename... Args>
  void run(Key key, sol::function fn, Args&&... args) {
    if (auto previous = coros.find(key); previous != coros.end()) {
      return;
    }
    auto it = threads.find(key);
    // We need two lookups, since we only want to create a thread if one is not
    // present
    if (it == threads.end()) {
      it = threads.emplace(key, sol::thread::create(state.lua_state())).first;
    }
    sol::state_view runner_state = it->second.state();
    sol::coroutine coro(runner_state, fn);
    auto result = coro(std::forward<Args>(args)...);
    if (!result.valid()) {
      sol::error err = result;
      ERROR_LOG("Lua error: {}", err.what());
      threads.erase(key);
    } else if (coro.runnable()) {
      coros.emplace(key, coro);
    }
  }

  void cancel(const Key& key) {
    coros.erase(key);
    threads.erase(key);
  }

  void tick_all() {
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

private:
  sol::state_view state;
  ska::flat_hash_map<Key, sol::coroutine> coros;
  ska::flat_hash_map<Key, sol::thread> threads;
};

// Holds an additional value to forward
// to the scheduler when running functions.
template <typename Key, typename Val>
class SchedulerValueAdapter {
public:
  SchedulerValueAdapter(CoroutineScheduler<Key>& scheduler)
      : scheduler(scheduler) {}

  void submit(Key key, Val value) { values.emplace(key, std::move(value)); }
  void remove(const Key& id) {
    values.erase(id);
    scheduler.cancel(id);
  }

  template <typename... Args>
  void run(const Key& key, sol::function fn, Args&&... args) {
    if (auto it = values.find(key); it != values.end()) {
      scheduler.run(key, fn, it->second, std::forward<Args>(args)...);
    } else {
      scheduler.run(key, fn, std::forward<Args>(args)...);
    }
  }

private:
  ska::flat_hash_map<Key, Val> values;
  CoroutineScheduler<Key>& scheduler;
};
} // namespace script
