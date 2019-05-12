#pragma once

#include <string>

#include "core/anim/text-drawl.hpp"
#include "core/render/font.hpp"
#include "core/resource/resource.hpp"

#include "widget.hpp"

namespace ui {
class Text : public Widget {
public:
  Text(std::string text, render::TexFrame frame);
  Text(std::string text, double speed, render::TexFrame frame);

  void update(double dt) override;
  void draw(UiContext& context) const override;

private:
  anim::TextDrawl drawl;
};
} // namespace ui
