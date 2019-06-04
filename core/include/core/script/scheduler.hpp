#pragma once

#include <vector>
#include <iostream>

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
  void run(sol::coroutine coro, Args&&... args) {
    coro(std::forward<Args>(args)...);
    if (coro.runnable()) {
			std::cout << "pushed a coro on the poop\n";
      scheduled.push_back(coro);
    }
  }

private:
  std::vector<sol::coroutine> scheduled;
};
} // namespace script
