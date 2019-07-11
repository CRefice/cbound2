#pragma once

#include <ssm/vector.hpp>

// An immutable rectangle whose dimensions are stored as vectors of type T.
template <typename T>
class Rectangle {
public:
  Rectangle() = default;

  Rectangle(ssm::vector<T, 2> p1, ssm::vector<T, 2> p2) {
    T left = p1.x < p2.x ? p1.x : p2.x;
    T right = p1.x < p2.x ? p2.x : p1.x;
    T bottom = p1.y < p2.y ? p1.y : p2.y;
    T top = p1.y < p2.y ? p2.y : p1.y;
    bl = ssm::vector<T, 2>(left, bottom);
    tr = ssm::vector<T, 2>(right, top);
  }

  Rectangle(T left, T right, T top, T bottom)
      : bl(left, bottom), tr(right, top) {}

  auto pos() const { return bl; }
  auto size() const { return tr - bl; }
  auto bottom_left() const { return bl; }
  auto bottom_right() const { return ssm::vector<T, 2>(tr.x, bl.y); }
  auto top_left() const { return ssm::vector<T, 2>(bl.x, tr.y); }
  auto top_right() const { return tr; }

  T width() const { return tr.x - bl.x; }
  T height() const { return tr.y - bl.y; }
  T area() const { return width() * height(); }

  T left() const { return bl.x; }
  T right() const { return tr.x; }
  T bottom() const { return bl.y; }
  T top() const { return tr.y; }

private:
  ssm::vector<T, 2> bl, tr;
};

template <typename T>
Rectangle<T> translate(const Rectangle<T>& rect, ssm::vector<T, 2> pos) {
  return Rectangle(rect.bottom_left() + pos, rect.top_right() + pos);
}
