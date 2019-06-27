#pragma once

#include <hash/flat_hash_map.hpp>

#include "common/rectangle.hpp"

#include "scene.hpp"
#include "entity.hpp"

namespace ecs {
struct Collision {
  // The bounds of the collider, relative
  // to the entity's position.
  Rectangle<float> bounds;
};

class CollisionManager {
public:
  void submit(EntityId id, Collision coll);

  void update(double dt, Scene& scene);

private:
	bool collides(const Collision& coll, const ssm::vec2& pos, const Scene& scene);

  ska::flat_hash_map<EntityId, Collision> collisions;
	double carry_dt = 0.0;
};
} // namespace ecs
