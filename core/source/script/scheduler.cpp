#include <algorithm>

#include "script/scheduler.hpp"

namespace script {
void Scheduler::tick() {
  // Remove finished coroutines
  auto end = std::remove_if(
      scheduled.begin(), scheduled.end(),
      [](const sol::coroutine& coro) { return !coro.runnable(); });
  scheduled.erase(end, scheduled.end());

  // Execute scheduled coroutines
  for (auto& coro : scheduled) {
    coro();
  }
}
} // namespace script
