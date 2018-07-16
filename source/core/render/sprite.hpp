#pragma once

#include <glad/glad.h>
#include <ssm/vector.hpp>

// How the four corners of a sprite's rectangle
// are represented in memory.
struct SpriteVertex
{
	using uvtype = ssm::vector<GLushort, 2>;

	//SpriteVertex(ssm::vec2 pos, uvtype uv) : pos(pos), uvtype(uv) {}
	ssm::vec2 pos;
	uvtype uv;
};

// A "view" into a texture.
// Points to a texture resource (don't know how yet)
// and has two texture coordinates, that define a
// rectangle over the texture.
struct Sprite
{
	//Texture texture;

	struct TexCoords {
		typename SpriteVertex::uvtype top_left, bottom_right;
	} texcoords;
};

