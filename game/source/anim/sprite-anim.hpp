#pragma once

#include <vector>

#include "common/rectangle.hpp"

// Changes behavior when reaching the end of the animation
enum class PlayMode
{
	PlayOnce, // Remain on the last frame
	Repeat, // Loop back to the beginning
	PingPong // Play the animation back in reverse
};

struct AnimSettings
{
	PlayMode mode;
	// How long does a single frame take.
	double frame_time_ms;
};

// A Sprite adapter that contains a list
// of animation frames and sets its tex coordinates
// accordingly.
class Animation
{
public:
	Animation(std::vector<Rectangle<int>> frames, AnimSettings settings) 
		: frames(std::move(frames)), settings(settings) {}

	void progress(double dt);

	Rectangle<int> current_frame();

private:
	AnimSettings settings;
	std::vector<Rectangle<int>> frames;
};
