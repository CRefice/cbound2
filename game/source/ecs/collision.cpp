#include <algorithm>
#include <cmath>

#include "core/collision/collision.hpp"

#include "collision.hpp"

namespace ecs {

using namespace ::render;
static const double fixed_update_dt = 0.016666666666667;

ssm::ivec2 ceil(const ssm::vec2 &a) {
  return ssm::ivec2(std::ceil(a.x), std::ceil(a.y));
}

ssm::ivec2 invert_y(ssm::ivec2 a, const ssm::ivec2 &size) {
  a.y = size.y - a.y;
  return a;
}

Rectangle<int> subtile_rect_from_id(const TileSet &set, int id) {
  auto tile_pos = ssm::ivec2(id % set.size.x, id / set.size.x);
  auto subtile_pos = tile_pos * set.collision.subtile_res;
  return Rectangle(subtile_pos, subtile_pos + set.collision.subtile_res);
}

Rectangle<int> tile_rect_from_rect(const TileMap &map,
                                   const Rectangle<float> &rect) {
  auto start_coord = ssm::ivec2(rect.pos() / map.tile_size);
  auto end_coord = ceil(rect.top_right() / map.tile_size);
  return Rectangle(invert_y(start_coord, map.size),
                   invert_y(end_coord, map.size));
}

static bool tilemap_collision(const Rectangle<float> &rect,
                              const Scene &scene) {
  const auto &map = *scene.tile_map;
  const auto &set = *scene.tile_set;
  auto start_tile_coord = ssm::ivec2(rect.pos() / map.tile_size);
  auto start_tile_pos = ssm::vec2(start_tile_coord) * map.tile_size;
  auto overlap_rect = tile_rect_from_rect(map, rect);
  auto subtile_size = map.tile_size / ssm::vec2(set.collision.subtile_res);

  for (const auto &layer : map.layers) {
    auto view = MatrixView(layer.tiles, overlap_rect);
    for (auto x = 0ul; x < view.width(); ++x) {
      for (auto y = 0ul; y < view.height(); ++y) {
        auto tile_coord =
            invert_y(ssm::ivec2(x, y), view.size()) - ssm::ivec2(0, 1);
        auto tile = view[tile_coord];
        if (tile == 0) {
          continue;
        }
        tile--;

        auto tile_pos = start_tile_pos + ssm::vec2(x, y) * map.tile_size;
        auto tile_rect =
            Rectangle(ssm::vec2(tile_pos), ssm::vec2(tile_pos + map.tile_size));

        auto tile_inter = *collision::intersection(tile_rect, rect);

        // Move to tile_based coordinates
        tile_inter = translate(tile_inter, -tile_pos);
        auto subtile_rel_start =
            ssm::ivec2(tile_inter.bottom_left() / subtile_size);
        auto subtile_rel_end = ceil(tile_inter.top_right() / subtile_size);

        auto sub_rect = subtile_rect_from_id(set, tile);

        auto sub_start = sub_rect.bottom_left() +
                         invert_y(subtile_rel_start, set.collision.subtile_res);
        auto sub_end = sub_rect.bottom_left() +
                       invert_y(subtile_rel_end, set.collision.subtile_res);

        sub_rect = Rectangle(sub_start, sub_end);
        auto sub_view = MatrixView(set.collision.data, sub_rect);

        if (std::any_of(sub_view.begin(), sub_view.end(),
                        [](const auto &i) { return (bool)i; })) {
          return true;
        }
      }
    }
  }

  return false;
}

void CollisionManager::submit(EntityId id, Collision coll) {
  collisions.emplace(id, coll);
}

void CollisionManager::remove(EntityId id) { collisions.erase(id); }

void CollisionManager::update(double dt, Scene &scene, BehaviorManager &behav) {
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
      behav.run(id1, *coll1.on_collision, coll2.tag);
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
         tilemap_collision(rect, scene);
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
} // namespace ecs
