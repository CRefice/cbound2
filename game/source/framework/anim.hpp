#pragma once

#include <optional>

#include <sol/forward.hpp>

#include "core/anim/sequence.hpp"
#include "core/render/tex-coords.hpp"

namespace fw::anim {
// Parse an animation sequence from the given table
std::optional<::anim::Sequence<::render::TexFrame>> parse_sequence(const sol::table& table);
} // namespace fw::anim
