#pragma once

#include "common\vec2.hpp"


#include "core\resource\resource.hpp"
#include "core\resource\shader.hpp"

#include "sprite.hpp"

namespace sprite_renderer
{
	void init(Resource<Shader> shader);
	void draw(Sprite& sprite, const vec2<uint>& pos);
};
