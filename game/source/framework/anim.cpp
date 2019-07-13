#include <vector>

#include <sol/sol.hpp>

#include "common/logging.hpp"

#include "core/script/script.hpp"

#include "anim.hpp"

namespace fw::anim {
using namespace ::anim;
using ::render::TexFrame;

void load_libraries(sol::state &state) {
  // Sequence types
  state.new_enum("play_mode", "once", PlayMode::Once, "loop", PlayMode::Loop,
                 "ping_pong", PlayMode::PingPong);
}

std::vector<Frame<TexFrame>> parse_frames(const sol::table &table) {
  std::vector<Frame<TexFrame>> frames;
  frames.reserve(table.size());
  for (const auto &[key, val] : table) {
    const sol::table &tbl = val;
    double dur = tbl.get_or("duration", 0.016); // Default duration of one frame
    if (auto rect = tbl.get<sol::optional<Rectangle<int>>>("coords")) {
      frames.push_back({*rect, dur});
    } else {
      WARN_LOG("Animation frame data contains non-frame data");
    }
  }
  return frames;
}

// Sequence format:
// [ mode = Loop ]
// { frames: TexFrame }...
std::optional<Sequence<TexFrame>> parse_sequence(const sol::table &table) {
  auto frames = parse_frames(table["frames"]);
  auto mode = table.get_or("mode", PlayMode::Loop);
  return Sequence<TexFrame>{std::move(frames), mode};
}
} // namespace fw::anim
