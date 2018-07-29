#pragma once

#include <vector>

#include "common/rectangle.hpp"

namespace anim {
// Changes behavior when reaching the end of the animation
enum class PlayMode
{
	PlayOnce, // Remain on the last frame
	Repeat, // Loop back to the beginning
	PingPong // Play the animation back in reverse
};

using Frame = Rectangle<int>;

struct Sequence
{
	std::vector<Frame> frames;
	PlayMode mode;
	double frame_time_ms;
};

inline double duration(const Sequence& seq) {
	return seq.frames.size() * seq.frame_time_ms;
}
}
