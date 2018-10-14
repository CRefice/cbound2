#pragma once

#include <glad/glad.h>
#include <ssm/vector.hpp>

#include "glinterface/common.hpp"
#include "glinterface/stream-buffer.hpp"

#include "core/resource/resource-cache.hpp"

#include "tex-coords.hpp"
#include "texture.hpp"
#include "sprite.hpp"

namespace render {
// A batch of drawing commands, uploaded to the GPU
// all at once.
class SpriteBatch
{
public:
	using size_type = GLushort;
	// @param batch_size: the maximum number of sprites
	// that will be drawn in this batch.
	// The size should be less than or equal to 
	// std::numeric_limits<size_type>::max() / 6,
	// since 6 indices are required to draw one sprite.
	// @param resources: the resource cache to load textures from.
	SpriteBatch(size_type batch_size, ResourceCache<Texture>& resources);

	// Add the sprite to the batch.
	// If the sprite's texture is different than the one
	// this function was last called with, the batch will be flushed.
	// Therefore, for maximum performance, draw sprites that use
	// the same texture as much as possible.
	// @param sprite: the sprite to be drawn.
	// @param pos: the top-left position of the resulting drawn rectangle.
	// @param color: color to multiply the sprite by. Defaults to white.
	void draw(const Sprite& sprite, const ssm::vec2& pos, const ssm::vec4& color = ssm::vec4(1.0f));

	// Issue a draw call, and clear all vertex and index buffers.
	// Note that this will bind no shaders, so make sure to bind them
	// ad-hoc every time you call this function.
	void flush();

	// Issue a draw call, but keep vertices in memory.
	// Useful for drawing the same stuff multiple times.
	void issue_draw_call();

private:
	// How the four corners of a sprite's rectangle
	// are represented in memory.
	// Note that the uvs are in normalized tex coords.
	struct SpriteVertex
	{
		ssm::vec2 pos;
		texture_coord uv;
		ssm::vec4 color;
	};

	gl::VertexArrayObject vao;
	gl::BufferObject vert_buffer, index_buffer;
	gl::BufferStream<SpriteVertex> vertices;
	gl::BufferStream<size_type> indices;

	ResourceCache<Texture>& resources;
	Resource<Texture> batch_texture;
};
}
