#pragma once

#include <optional>

#include <sol/sol.hpp>
#include <ssm/vector.hpp>

#include "common/rectangle.hpp"

namespace script {
template <typename T>
std::optional<ssm::vector<T, 2>> parse_vec2(const sol::table& table, int index = 1) {
	if (table == sol::nil) return {};
	auto [x, y] = table.get<sol::optional<T>, sol::optional<T>>(index, index + 1);
	if (!x || !y) {
		return {};
	}
	return ssm::vector<T, 2>(*x, *y);
}

template <typename T>
std::optional<Rectangle<T>> parse_rect(const sol::table& table) {
	if (table == sol::nil) return {};
	auto maybe_pt1 = parse_vec2<T>(table, 1);
	auto maybe_pt2 = parse_vec2<T>(table, 3);
	if (!maybe_pt1 || !maybe_pt2) {
		return {};
	}
	return Rectangle(*maybe_pt1, *maybe_pt2);
}

// Create a new environment
// that a singular script can be sourced in.
sol::state new_environment();

// Define common types and functions in the given state.
void load_common_libs(sol::state& table);
} // namespace script
