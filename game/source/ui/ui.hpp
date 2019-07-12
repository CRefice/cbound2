#pragma once

#include "core/render/font.hpp"
#include "core/render/sprite-batch.hpp"
#include "core/resource/resource-cache.hpp"

namespace ui {
struct UiContext {
  render::SpriteBatch& sprite_batch;
  ResourceCache<render::Font>& fonts;
};
} // namespace ui
