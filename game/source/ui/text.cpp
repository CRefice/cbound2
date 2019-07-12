#include "core/render/text-batch.hpp"

#include "text.hpp"

using namespace render;

namespace ui {
Text::Text(std::string_view text, double speed, ssm::vec2 sz, std::string font)
    : Widget(sz), font(font), params{{}, sz.x},
      drawl(render::split_whitespace(text, params), speed) {}

Text::Text(std::string_view text, ssm::vec2 sz, std::string font)
    : Text(text, 0.02, sz, font) {
  drawl.skip();
}

void Text::update(double dt) { drawl.progress(dt); }

void Text::draw(ssm::vec2 pos, UiContext& ctx) const {
	auto draw_params = params;
  draw_params.font = ctx.fonts.load(font);
  render::TextBatch batch(ctx.sprite_batch, draw_params);
  batch.draw(drawl.current_slice(), pos);
}
} // namespace ui
