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
		if (auto rect = script::parse_rect<int>(val)) {
			frames.emplace_back(*rect);
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
	auto frame_time = table.get_or("frame_time", 1.0 / 60);
	return Sequence{std::move(frames), mode, frame_time };
}
} // namespace fw::anim
