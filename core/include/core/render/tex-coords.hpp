#pragma once

#include <limits>
#include <ssm/vector.hpp>

#include "common/rectangle.hpp"

namespace render {
using PixelCoord = ssm::vector<int, 2>;
using TexCoord = ssm::vector<unsigned short, 2>;

// A "window" over a texture used for rendering, in pixel coordinates.
using TexFrame = Rectangle<int>;
using UvFrame = Rectangle<unsigned short>;

// Convert from pixel texture coordinates to normalized integer UV coordinates.
inline TexCoord normalize(const PixelCoord& coord, const PixelCoord& size) {
	auto x = coord.x * std::numeric_limits<unsigned short>::max() / size.x;
	auto y = coord.y * std::numeric_limits<unsigned short>::max() / size.y;
	return TexCoord(x, y);
}

template <typename T>
Rectangle<float> convert(Rectangle<T> frame) {
	float l = frame.left();
	float r = frame.right();
	float t = frame.top();
	float b = frame.bottom();
	return Rectangle<float>(l, r, t, b);
}
}
