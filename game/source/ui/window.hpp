#pragma once

#include "widget.hpp"

namespace ui {
class Window : public Widget {
public:
  Window(ssm::vec2 sz) : Widget(sz) {}

  void draw(ssm::vec2 pos, UiContext& context) const override;
};
} // namespace ui
