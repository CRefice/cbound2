#pragma once

#include <optional>

#include <sol.hpp>

#include "core/anim/sequence.hpp"

namespace framework {
std::optional<anim::Sequence> parse_sequence(const sol::table& table);
}
