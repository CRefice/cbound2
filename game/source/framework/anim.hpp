#pragma once

#include <optional>

#include <sol.hpp>

#include "core/anim/sequence.hpp"

namespace fw::anim {
// Add all the framework definitions to the given state.
void load_libraries(sol::state& state);

// Parse an animation sequence from the given table
std::optional<::anim::Sequence> parse_sequence(const sol::table& table);
} // namespace fw::anim
