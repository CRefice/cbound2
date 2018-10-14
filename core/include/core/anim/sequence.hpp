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

// An animation keyframe, that can contain any kind of value.
template <typename T>
struct Frame
{
	T value;
	// The time this frame takes, in seconds
	double duration;
};

// An sequence of keyframes.
template <typename T>
struct Sequence
{
	// The actual array of frames
	std::vector<Frame<T>> frames;
	// What to do on animation end
	PlayMode mode;
};
}
