#pragma once

#include "core/render/font.hpp"
#include "core/render/sprite-batch.hpp"
#include "core/resource/resource.hpp"

namespace ui {
struct UiContext {
  Resource<render::Font> font;
  render::SpriteBatch& sprite_batch;
};
} // namespace ui
