#include <iostream>

#include "core/render/text-batch.hpp"

#include "text.hpp"

using namespace render;

namespace ui {
Text::Text(std::string_view text, double speed, ssm::vec2 sz,
           Resource<Font> font)
    : Widget(sz), font(font), params{std::move(font), sz.x},
      drawl(render::split_whitespace(text, params), speed) {}

Text::Text(std::string_view text, ssm::vec2 sz, Resource<Font> font)
    : Text(text, 0, sz, std::move(font)) {
  drawl.skip();
}

void Text::update(double dt) { drawl.progress(dt); }

void Text::draw(ssm::vec2 pos, UiContext& ctx) const {
  render::TextBatch batch(ctx.sprite_batch, params);
  batch.draw(drawl.current_slice(), pos);
}
} // namespace ui
