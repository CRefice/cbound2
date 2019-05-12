#pragma once

#include "core/render/tex-coords.hpp"

#include "ui.hpp"

namespace ui {
class Widget {
public:
  Widget(render::TexFrame frame) : frm(frame) {}
  virtual ~Widget() = 0;

  render::TexFrame frame() const { return frm; }

  virtual void update(double dt) {}
  virtual void draw(UiContext& context) const = 0;

private:
  render::TexFrame frm;
};

inline Widget::~Widget() {}
} // namespace ui
