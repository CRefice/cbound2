#pragma once

#include <functional>
#include <vector>

#include <hash/flat_hash_map.hpp>

#include "core/input/key.hpp"

#include "entity.hpp"

namespace ecs {
using ActionCallback = std::function<void(EntityId)>;

struct InputContext {
  bool handle(EntityId id, ::input::KeyEvent event);

  ska::flat_hash_map<::input::KeyEvent, ActionCallback> actions;
};

class InputManager : public input::KeyHandler {
public:
  void submit(EntityId id, InputContext ctx) {
    contexts.emplace_back(id, std::move(ctx));
  }

  void handle(::input::KeyEvent event) final;

private:
  std::vector<std::pair<EntityId, InputContext>> contexts;
};
} // namespace ecs
