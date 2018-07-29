#include <vector>

#include "common/logging.hpp"

#include "core/script/common.hpp"

#include "anim.hpp"

using namespace anim;

namespace framework {
std::vector<Frame> parse_frames(const sol::table& table) {
	std::vector<Frame> frames;
	frames.reserve(table.size());
	for (std::size_t i = 0; i < table.size(); ++i) {
		if (auto rect = script::parse_rect<int>(table[i])) {
			frames.emplace_back(*rect);
		} else {
			WARN_LOG("Animation frame data contains non-frame data");
		}
	}
	return frames;
}

// Animation format:
// [ frame_time_ms = 1/60sec ]
// [ mode = PlayOnce ]
// { frames: Rectangle<int> }...
std::optional<Sequence> parse_animation(const sol::table& table) {
	// TODO: get mode, add it as an enum probably.
	auto frames = parse_frames(table["frames"]);
	if (frames.empty())
		return {};

	auto frame_time = table.get_or("frame_time", 1000.0 / 60);
	return Sequence{std::move(frames), PlayMode::PlayOnce, frame_time };
}
} // namespace framework
