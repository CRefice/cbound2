#pragma once

#include <cassert>

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
  const Movement& get_movement(EntityId id) const { return *data.find(id); }
  Movement& get_movement(EntityId id) { return *data.find(id); }

  EntityId submit(Movement mov) { return data.add(mov); }

  void update(double dt) {
    for (auto& mvmt : data) {
      mvmt.pos += (float)dt * mvmt.velocity;
    }
  }

private:
  SlotMap<Movement> data;
};
} // namespace ecs
