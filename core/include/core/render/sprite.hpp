#pragma once

#include <glad/glad.h>

#include "common/rectangle.hpp"
#include "../resource/resource.hpp"
#include "texture.hpp"

// How the four corners of a sprite's rectangle
// are represented in memory.
struct SpriteVertex
{
	ssm::vec2 pos;
	ssm::vector<GLushort, 2> uv;
};

// A "view" into a texture,
// with its own size and a rectangle
// of texture coordinates defining a "window" into it.
// Doesn't own the texture, but holds a resource to it.
struct Sprite
{
	Resource<Texture> texture;
	ssm::vec2 size;
	Rectangle<GLushort> tex_coords;
};
