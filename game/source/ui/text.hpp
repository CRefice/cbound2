#pragma once

#include <string>

#include "core/anim/text-drawl.hpp"
#include "core/render/font.hpp"
#include "core/resource/resource.hpp"

#include "widget.hpp"

namespace ui {
class Text : public Widget {
public:
  Text(std::string text, ssm::vec2 size);
  Text(std::string text, double speed, ssm::vec2 size);

  void update(double dt) override;
  void draw(ssm::vec2 pos, UiContext& context) const override;

  std::unique_ptr<Widget> clone() const override;

private:
  anim::TextDrawl drawl;
};
} // namespace ui
