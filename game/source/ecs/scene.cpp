#include "scene.hpp"

namespace ecs {
EntityId Scene::submit(Movement m) { return data.add(std::move(m)); }

const Movement* Scene::find(EntityId id) const {
  auto it = data.find(id);
  return it == data.end() ? nullptr : &*it;
}

Movement* Scene::find(EntityId id) {
  auto it = data.find(id);
  return it == data.end() ? nullptr : &*it;
}

void Scene::update(double dt) {
  for (auto& mvmt : data) {
    //mvmt.pos += mvmt.velocity * (float)dt;
  }
}

void Scene::remove(EntityId id) { data.remove(id); }
} // namespace ecs
