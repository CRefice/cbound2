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

struct Frame
{
	Rectangle<int> coords;
	// The time this frame takes, in seconds
	double duration;
};

struct Sequence
{
	// The actual array of frames
	std::vector<Frame> frames;
	// What to do on animation end
	PlayMode mode;
};
}
