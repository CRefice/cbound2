#pragma once

#include <optional>

#include <sol/sol.hpp>

#include "common/logging.hpp"

#include "core/anim/sequence.hpp"
#include "core/anim/sequencer.hpp"
#include "core/render/tex-coords.hpp"

namespace fw::anim {
template <typename T>
void register_sequencer(sol::table table, const char* name) {
  using Seq = ::anim::Sequencer<T>;
  auto meta = table.new_usertype<Seq>(name, sol::no_constructor);

  meta["restart"] = &Seq::restart;
  meta["fast_forward"] = &Seq::fast_forward;
  meta["pause"] = &Seq::pause;
  meta["resume"] = &Seq::resume;
}

template <typename T>
std::vector<::anim::Frame<T>> parse_frames(const sol::table& table) {
  constexpr auto frame_dur = 1.0 / 60.0;
  std::vector<::anim::Frame<T>> frames;
  frames.reserve(table.size());
  for (const auto& elem : table) {
    const sol::table& tbl = elem.second;
    double dur = tbl.get_or("duration", frame_dur);
    if (auto rect = tbl.get<sol::optional<T>>("value")) {
      frames.push_back({*rect, dur});
    } else {
      WARN_LOG("Invalid animation data");
    }
  }
  return frames;
}

// Parse an animation sequence from the given table
// Sequence format:
// [ mode = Loop ]
// { frames: TexFrame }...
template <typename T>
std::optional<::anim::Sequence<T>> parse_sequence(const sol::table& table) {
  auto frames = parse_frames<T>(table["frames"]);
  auto mode = table.get_or("mode", ::anim::PlayMode::Loop);
  return ::anim::Sequence<T>{std::move(frames), mode};
}

inline void register_libs(sol::state_view state) {
  // Sequence types
  auto table = state.create_named_table("anim");
  table.new_enum("play_mode", "once", ::anim::PlayMode::Once, "loop",
                 ::anim::PlayMode::Loop, "ping_pong",
                 ::anim::PlayMode::PingPong);
  register_sequencer<render::TexFrame>(table, "Sequencer");
}
} // namespace fw::anim
