#pragma once

#include <limits>

#include <ssm/vector.hpp>

namespace render {
using pixel_coord = ssm::vector<int, 2>;
using texture_coord = ssm::vector<unsigned short, 2>;

// Convert from pixel texture coordinates to normalized integer UV coordinates.
inline texture_coord normalize(const pixel_coord& coord, const pixel_coord& size) {
	auto x = coord.x * std::numeric_limits<unsigned short>::max() / size.x;
	auto y = coord.y * std::numeric_limits<unsigned short>::max() / size.y;
	return texture_coord(x, y);
}
}
