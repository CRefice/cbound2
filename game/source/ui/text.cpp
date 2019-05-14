#include "core/render/text-batch.hpp"

#include "text.hpp"

namespace ui {
Text::Text(std::string text, double speed, ssm::vec2 sz)
    : Widget(sz), drawl(text, speed) {}

Text::Text(std::string text, ssm::vec2 sz) : Text(text, 0.02, sz) {
  drawl.skip();
}

void Text::update(double dt) { drawl.progress(dt); }

void Text::draw(ssm::vec2 pos, UiContext& ctx) const {
  render::TextDrawParams params{ctx.font};
  params.max_width = size().x;

  render::TextBatch batch(ctx.sprite_batch, std::move(params));
  batch.draw(drawl.current_slice(), pos);
}

std::unique_ptr<Widget> Text::clone() const {
  return std::make_unique<Text>(*this);
}
} // namespace ui
