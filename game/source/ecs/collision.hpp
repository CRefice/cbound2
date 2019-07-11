#pragma once

#include <hash/flat_hash_map.hpp>
#include <sol/sol.hpp>

#include "common/rectangle.hpp"

#include "behavior.hpp"
#include "entity.hpp"
#include "scene.hpp"

namespace ecs {
struct Collision {
  sol::optional<sol::function> on_collision;
  bool solid;
  // The bounds of the collider, relative
  // to the entity's position.
  Rectangle<float> bounds;
};

class CollisionManager {
public:
  void submit(EntityId id, Collision coll);

  void update(double dt, Scene& scene, BehaviorManager& behav);

private:
  using iterator = typename ska::flat_hash_map<EntityId, Collision>::iterator;

  bool scene_collision(const Collision& coll, const ssm::vec2& pos,
                       const Scene& scene);

  iterator overlap_entity(const EntityId& id, const Collision& coll,
                          const ssm::vec2& pos, const Scene& scene);

  void apply_velocity(const EntityId& id, const Collision& coll,
                      const ssm::vec2& vel, Scene& scene,
                      BehaviorManager& behav);

  ska::flat_hash_map<EntityId, Collision> collisions;
  double carry_dt = 0.0;
};
} // namespace ecs
