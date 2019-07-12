#pragma once

#include <hash/flat_hash_map.hpp>
#include <sol/sol.hpp>
#include <unordered_set>

#include "common/hashes.hpp"
#include "common/rectangle.hpp"

#include "behavior.hpp"
#include "entity.hpp"
#include "scene.hpp"

namespace ecs {
struct Collision {
  sol::optional<sol::function> on_collision;
  bool solid;
  std::string tag;
  // The bounds of the collider, relative
  // to the entity's position.
  Rectangle<float> bounds;
};

class CollisionManager {
public:
  void submit(EntityId id, Collision coll);
  void remove(EntityId id);

  void update(double dt, Scene& scene, BehaviorManager& behav);

private:
  using iterator = typename ska::flat_hash_map<EntityId, Collision>::iterator;
  using PairSet =
      std::unordered_set<std::pair<EntityId, EntityId>, ordered_pair_hash>;

  bool scene_collision(const Collision& coll, const ssm::vec2& pos,
                       const Scene& scene);

  iterator overlap_entity(const EntityId& id, const Collision& coll,
                          const ssm::vec2& pos, const Scene& scene);

  void apply_velocity(const EntityId& id, Collision& coll, const ssm::vec2& vel,
                      Scene& scene);

  PairSet collision_set;
  ska::flat_hash_map<EntityId, Collision> collisions;
  double carry_dt = 0.0;
};
} // namespace ecs
