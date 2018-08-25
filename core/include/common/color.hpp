#pragma once

#include <optional>

#include <ssm/vector.hpp>

namespace color {
	std::optional<ssm::vec3> parse_color(std::string_view str);
}
