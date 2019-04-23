#pragma once

#include <ssm/vector.hpp>

#include "common/slot-map.hpp"

#include "entity.hpp"

namespace ecs {
struct Movement {
  ssm::vec2 pos, velocity;
};

// Holds the movement components of all entities.
class Scene {
public:
  const Movement* find(EntityId id) const;
  Movement* find(EntityId id);

  EntityId submit(Movement mov);
  void update(double dt);

private:
  SlotMap<Movement> data;
};
} // namespace ecs
