#pragma once

#include <optional>

#include <sol.hpp>
#include <ssm/vector.hpp>

#include "common/rectangle.hpp"

namespace script {
template <typename T>
std::optional<Rectangle<T>> parse_rect(const sol::table& table) {
	if (table == sol::nil) return {};
	auto [x1, y1, x2, y2] = table.get(1, 2, 3, 4);
	if (x1 == sol::nil || y1 == sol::nil || x2 == sol::nil || y2 == sol::nil) {
		return {};
	}
	return Rectangle({ x1, y1 }, { x2, y2 });
}

template <typename T>
std::optional<ssm::vector<T, 2>> parse_vec2(const sol::table& table) {
	if (table == sol::nil) return {};
	auto [x, y] = table.get(1, 2);
	if (x == sol::nil || y == sol::nil) {
		return {};
	}
	return { x, y };
}

// Create a new environment
// that a singular script can be sourced in.
sol::state new_environment();
} // namespace script
