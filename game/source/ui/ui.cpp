#include "ui.hpp"

namespace ui {
UiContext::UiContext(ResourceCache<render::Texture>& textures)
    : sprite_batch(10000, textures),
      text_batch(sprite_batch, {fonts.load("fonts/font.fnt")}) {}

void UiContext::text_window(Rectangle<float> osc, std::string_view text) {
  // TODO: Draw window frame
  text_batch.set_max_width(osc.width());
  text_batch.draw(text, osc.top_left());
}

void UiContext::flush() { sprite_batch.flush(); }
} // namespace ui
