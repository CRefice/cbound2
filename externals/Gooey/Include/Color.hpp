#pragma once

#include <cstdint>

namespace Goo
{
struct Color
{
  uint8_t r, g, b;

  Color(uint8_t r, uint8_t g, uint8_t b) : r(r), g(g), b(b) {}

  static Color red();
  static Color green();
  static Color blue();

  static Color black();
  static Color white();

  static Color control();
};
}
