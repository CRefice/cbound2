#pragma once

#include <vector>

#include <hash/flat_hash_map.hpp>
#include <sol/sol.hpp>

#include "core/input/key.hpp"
#include "core/script/scheduler.hpp"

#include "entity.hpp"

namespace ecs {
using Scheduler = script::Scheduler;
using ActionCallback = sol::function;

struct InputContext {
  bool handle(const EntityId& id, ::input::KeyEvent event, Scheduler& sched);

  ska::flat_hash_map<::input::KeyEvent, ActionCallback> actions;
};

class InputManager : public input::KeyHandler {
public:
  InputManager(Scheduler& scheduler) : sched(scheduler) {}

  void submit(EntityId id, InputContext ctx) {
    contexts.emplace_back(id, std::move(ctx));
  }

  void remove(EntityId id);

  void delete_dead();

  void handle(::input::KeyEvent event) final;

private:
  Scheduler& sched;
  std::vector<std::pair<EntityId, InputContext>> contexts;
  std::size_t to_delete = 0;
};
} // namespace ecs
