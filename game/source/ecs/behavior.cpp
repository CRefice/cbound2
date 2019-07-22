#include "behavior.hpp"

namespace ecs {
void BehaviorManager::update(double dt, BehaviorRunner& runner) {
  for (const auto& pair : behaviors) {
    runner.run(pair.first, pair.second, dt);
  }
}

void BehaviorManager::load_entity(const EntityId& id, sol::table& table) {
  if (auto update = table.get<sol::optional<sol::function>>("update")) {
    submit(id, *update);
  }
}
} // namespace ecs
