#pragma once

#include <optional>

#include <sol/sol.hpp>

#include "common/logging.hpp"

#include "core/anim/sequence.hpp"
#include "core/anim/sequencer.hpp"

#include "framework.hpp"

namespace fw {
template <typename T>
std::vector<anim::Frame<T>> parse_frames(const sol::table& table) {
  constexpr auto frame_dur = 1.0 / 60.0;
  std::vector<anim::Frame<T>> frames;
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

template <typename T>
struct LuaTraits<anim::Sequencer<T>> {
  using Seq = anim::Sequencer<T>;

  static std::optional<Seq> parse(const sol::table& table) {
    auto frames = parse_frames<T>(table["frames"]);
    auto mode = table.get_or("mode", anim::PlayMode::Loop);
    auto seq = anim::Sequence<T>{frames, mode};
    auto speed = table.get_or("speed", 1.0);
    return Seq(seq, speed);
  }

  static void bind(sol::table& table, const char* name) {
    auto meta = table.new_usertype<Seq>(name, sol::no_constructor);
    meta["restart"] = &Seq::restart;
    meta["skip_to"] = &Seq::skip_to;
    meta["fast_forward"] = &Seq::fast_forward;
    meta["pause"] = &Seq::pause;
    meta["resume"] = &Seq::resume;
    meta["current_time"] = &Seq::current_time;
  }
};

template <>
struct LuaTraits<anim::PlayMode> {
  static void bind(sol::table& table, const char* name) {
    table.new_enum(name, "once", anim::PlayMode::Once, "loop",
                   anim::PlayMode::Loop, "ping_pong", anim::PlayMode::PingPong);
  }
};

void bind_anim_libs(sol::state_view state);
} // namespace fw
