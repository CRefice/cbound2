#pragma once

#include <cassert>
#include <vector>

#include <ssm/vector.hpp>

#include "entity.hpp"

namespace ecs {
// Holds the positions of all entities.
class Scene {
public:
  ssm::vec2 get_position(EntityId id) const { return positions[id]; }
  ssm::vec2& get_position(EntityId id) { return positions[id]; }

  void submit(EntityId id, ssm::vec2 pos) {
		assert(positions.size() == id);
		positions.push_back(pos);
	}

private:
  // Every entity has a position, so we can just index this vector
  // by entity ID.
  std::vector<ssm::vec2> positions;
};
} // namespace ecs
