#pragma once

#include <vector>

#include <sol/sol.hpp>

namespace script {
// A simple scheduler for coroutines.
// Stores references to unfinished coroutines,
// and runs them when 'tick' is called.
// If a coroutine is finished, it removes it
// from its scheduled list
class Scheduler {
public:
  void tick();

  template <typename... Args>
  void run(sol::function fn, Args&&... args) {
    sol::thread runner = sol::thread::create(fn.lua_state());
    sol::state_view runner_state = runner.state();
    sol::coroutine coro(runner_state, fn);
    coro(std::forward<Args>(args)...);
    if (coro.runnable()) {
      scheduled.push_back(coro);
    }
  }

private:
  std::vector<sol::coroutine> scheduled;
};
} // namespace script
