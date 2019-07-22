#include <algorithm>
#include <cmath>

#include "common/logging.hpp"

#include "core/collision/collision.hpp"

#include "framework/collision.hpp"

#include "collision.hpp"

namespace ecs {
static const double fixed_update_dt = 0.016666666666667;

void CollisionManager::update(double dt, Scene &scene, BehaviorRunner &runner) {
  carry_dt += dt;
  collision_set.clear();
  while (carry_dt >= fixed_update_dt) {
    for (auto &[id, coll] : collisions) {
      auto mvmt = scene.find(id);
      auto horiz_vel = mvmt->velocity;
      horiz_vel.y = 0.0f;
      apply_velocity(id, coll, horiz_vel, scene);
      auto vert_vel = mvmt->velocity;
      vert_vel.x = 0.0f;
      apply_velocity(id, coll, vert_vel, scene);
    }
    carry_dt -= fixed_update_dt;
  }

  for (auto &[id1, id2] : collision_set) {
    auto &coll1 = collisions[id1];
    auto &coll2 = collisions[id2];
    if (coll1.on_collision) {
      runner.run(id1, *coll1.on_collision, coll2.tag);
    }
  }
}

bool CollisionManager::scene_collision(const Collision &coll,
                                       const ssm::vec2 &pos,
                                       const Scene &scene) {
  auto map_bounds = bounds(*scene.tile_map);
  auto rect = translate(coll.bounds, pos);
  return rect.left() < 0.0f || rect.right() > map_bounds.right() ||
         rect.bottom() < 0.0f || rect.top() > map_bounds.top() ||
         collision::tile_collision(rect, *scene.tile_map, *scene.tile_set);
}

CollisionManager::iterator
CollisionManager::overlap_entity(const EntityId &id, const Collision &coll,
                                 const ssm::vec2 &pos, const Scene &scene) {
  auto rect = translate(coll.bounds, pos);
  return std::find_if(collisions.begin(), collisions.end(), [&](const auto &x) {
    const auto &[id2, coll] = x;
    if (id == id2)
      return false;
    auto pos2 = scene.find(id2)->pos;
    auto rect2 = translate(coll.bounds, pos2);
    return collision::overlap(rect, rect2);
  });
}

void CollisionManager::apply_velocity(const EntityId &id, Collision &coll,
                                      const ssm::vec2 &vel, Scene &scene) {
  auto mvmt = scene.find(id);
  auto &pos = mvmt->pos;
  auto old_pos = pos;
  pos += vel * (float)fixed_update_dt;
  if (scene_collision(coll, pos, scene)) {
    pos = old_pos;
  } else if (auto other = overlap_entity(id, coll, pos, scene);
             other != collisions.end()) {
    const auto &[id2, coll2] = *other;
    if (coll2.solid) {
      pos = old_pos;
    }
    collision_set.emplace(id, id2);
    collision_set.emplace(id2, id);
  }
}

void CollisionManager::load_entity(const EntityId &id, sol::table &entity) {
  if (auto table = entity["collision"]) {
    if (auto coll = fw::LuaTraits<Collision>::parse(table)) {
      submit(id, *coll);
      table = sol::nil;
    } else {
      WARN_LOG("Unable to parse collision component!");
    }
  }
}
} // namespace ecs
