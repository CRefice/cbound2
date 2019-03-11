#pragma once

#include <cassert>

#include <ssm/vector.hpp>

#include "common/slot-map.hpp"

#include "entity.hpp"

namespace ecs {
// Holds the positions of all entities.
class Scene {
public:
  ssm::vec2 get_position(EntityId id) const { return *positions.find(id); }
  ssm::vec2& get_position(EntityId id) { return *positions.find(id); }

  EntityId submit(ssm::vec2 pos) {
		return positions.add(pos);
	}

private:
  SlotMap<ssm::vec2> positions;
};
} // namespace ecs
