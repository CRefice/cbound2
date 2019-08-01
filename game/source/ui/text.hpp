#pragma once

#include <string_view>

#include "core/anim/text-drawl.hpp"
#include "core/render/font.hpp"
#include "core/resource/resource.hpp"

#include "widget.hpp"

namespace ui {
class Text : public Widget {
public:
  Text(std::string_view text, ssm::vec2 size,
       Resource<render::Font> font);
  Text(std::string_view text, double speed, ssm::vec2 size,
       Resource<render::Font> font);

  void update(double dt) override;
  void draw(ssm::vec2 pos, UiContext& context) const override;

  void skip() { drawl.skip(); }
  bool done() const { return drawl.done(); }

private:
	Resource<render::Font> font;
  render::TextDrawParams params;
  anim::TextDrawl drawl;
};
} // namespace ui
