#pragma once

#include <utility>

#include "common\inttypes.hpp"

#include "core\resource\resource.hpp"

#include "texture.hpp"

class Sprite
{
public:
	Sprite(const Resource<Texture>& texture) : tex(texture) {}
	Sprite(Resource<Texture>&& texture) : tex(std::move(texture)) {}

	const Resource<Texture>& texture() { return tex; }

private:
	Resource<Texture> tex;
	uint cur_frame = 0;
};
