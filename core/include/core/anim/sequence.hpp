#pragma once

#include <vector>

#include "common/rectangle.hpp"

namespace anim {
// Changes behavior when reaching the end of the animation
enum class PlayMode
{
	Once, // Remain on the last frame
	Loop, // Loop back to the beginning
	PingPong // Play the animation back in reverse
};

using Frame = Rectangle<int>;

struct Sequence
{
	// The actual array of frames
	std::vector<Frame> frames;
	// What to do on animation end
	PlayMode mode;
	// The time between two frames, in seconds
	double frame_time;
};

inline double duration(const Sequence& seq) {
	return seq.frames.size() * seq.frame_time;
}
}
