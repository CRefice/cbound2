#pragma once

#include <memory>

#include <hash/flat_hash_map.hpp>

#include "core/render/font.hpp"
#include "core/render/sprite-batch.hpp"
#include "core/render/sprite.hpp"
#include "core/render/texture.hpp"
#include "core/resource/resource-cache.hpp"

#include "ui/widget.hpp"

#include "entity.hpp"
#include "scene.hpp"

namespace ecs {
class UiManager {
public:
  UiManager(ResourceCache<render::Texture>& textures) : batch(10000, textures) {}

  void submit(EntityId id, std::unique_ptr<ui::Widget> widget);
  void remove(EntityId id);

  void update(double dt);
  void draw_all(const Scene& scene);

private:
  ska::flat_hash_map<EntityId, std::unique_ptr<ui::Widget>> widgets;

  render::SpriteBatch batch;
  ResourceCache<render::Font> fonts;
};
} // namespace ecs
