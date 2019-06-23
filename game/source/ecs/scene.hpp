#pragma once

#include <ssm/vector.hpp>

#include "common/slot-map.hpp"
#include "core/render/tiles.hpp"
#include "core/resource/resource.hpp"

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
  void remove(EntityId id);

  void update(double dt);

  Resource<render::TileMap> tile_map;
  Resource<render::TileSet> tile_set;

private:
  SlotMap<Movement> data;
};
} // namespace ecs
