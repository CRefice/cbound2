#include "core/render/text-batch.hpp"

#include "text.hpp"

namespace ui {
Text::Text(std::string text, double speed, render::TexFrame frame)
    : Widget(frame), drawl(text, speed) {}

Text::Text(std::string text, render::TexFrame frame) : Text(text, 0.02, frame) {
  drawl.skip();
}

void Text::update(double dt) { drawl.progress(dt); }

void Text::draw(UiContext& ctx) const {
	auto frm = render::convert(frame());
	render::TextDrawParams params { ctx.font };
	params.max_width = frm.width();

	render::TextBatch batch(ctx.sprite_batch, std::move(params));
	batch.draw(drawl.current_slice(), frm.top_left());
}
} // namespace ui
