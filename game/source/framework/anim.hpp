#pragma once

#include <string>
#include <vector>
#include <unordered_map>

#include "common/rectangle.hpp"

struct Anim
{
	double frame_time_ms;
	std::vector<Rectangle<int>> frames;
};

struct AnimComponent
{

	std::unordered_map<std::string, Anim>
};
