#pragma once

#include <glad/glad.h>

#include "common/rectangle.hpp"
#include "../resource/resource.hpp"
#include "texture.hpp"

// A "view" into a texture,
// with its own size and a rectangle
// of texture coordinates defining a "window" into it.
// Doesn't own the texture, but holds a resource to it.
// Originally, frame was in normalized texture coordinates.
// However, it was a pain in the ass to manually convert every time.
// Therefore, the conversion will be made by the sprite batch.
struct Sprite
{
	Resource<Texture> texture;
	ssm::vec2 size;
	Rectangle<int> frame;
};
