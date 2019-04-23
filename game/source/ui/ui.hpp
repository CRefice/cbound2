#pragma once

#include "common/rectangle.hpp"

#include "core/render/sprite-batch.hpp"
#include "core/render/text-batch.hpp"

namespace ui {
class UiContext {
public:
  UiContext(ResourceCache<render::Texture>& textures);

  // Place a window at the given on-screen pixel coordinates
  // containing the given text
  void text_window(Rectangle<float> osc, std::string_view text);

  // Submit all buffers to OpenGL for drawing.
	void flush();

private:
  ResourceCache<render::Font> fonts;
	render::SpriteBatch sprite_batch;
  render::TextBatch text_batch;
};
} // namespace ui
