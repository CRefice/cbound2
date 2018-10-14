#include <vector>

#include "common/logging.hpp"

#include "core/script/common.hpp"

#include "anim.hpp"

namespace fw::anim {
using namespace ::anim;

void load_libraries(sol::state& state) {
	// Sequence types
	state.new_enum("play_mode",
			"once", PlayMode::Once,
			"loop", PlayMode::Loop,
			"ping_pong", PlayMode::PingPong
			);
}

std::vector<Frame> parse_frames(const sol::table& table) {
	std::vector<Frame> frames;
	frames.reserve(table.size());
	for (const auto& [key, val] : table) {
		const sol::table& tbl = val;
		double dur = tbl.get_or("duration", 0.016); //Default duration of one frame
		if (auto rect = script::parse_rect<int>(tbl["coords"])) {
			frames.push_back(Frame{*rect, dur});
		} else {
			WARN_LOG("Animation frame data contains non-frame data");
		}
	}
	return frames;
}

// Sequence format:
// [ frame_time = 1/60sec ]
// [ mode = Loop ]
// { frames: Rectangle<int> }...
std::optional<Sequence> parse_sequence(const sol::table& table) {
	auto frames = parse_frames(table["frames"]);
	auto mode = table.get_or("mode", PlayMode::Loop);
	return Sequence{std::move(frames), mode };
}
} // namespace fw::anim
