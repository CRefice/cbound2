#pragma once

#include <memory>

#include <ssm/vector.hpp>

#include "ui.hpp"

namespace ui {
class Widget {
public:
  Widget(ssm::vec2 size) : sz(size) {}
  virtual ~Widget() = 0;

  ssm::vec2 size() const { return sz; }

  virtual void update(double dt) {}
  virtual void draw(ssm::vec2 pos, UiContext& context) const = 0;

private:
  ssm::vec2 sz;
};

inline Widget::~Widget() {}
} // namespace ui
