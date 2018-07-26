#pragma once

#include <glad/glad.h>
#include <ssm/vector.hpp>

#include "glinterface/common.hpp"
#include "glinterface/stream-buffer.hpp"

#include "sprite.hpp"

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
	SpriteBatch(size_type batch_size);

	// Add the sprite to the batch.
	// If the sprite's texture is different than the one
	// this function was last called with, the batch will be flushed.
	// Therefore, for maximum performance, draw sprites that use
	// the same texture as much as possible.
	// @param sprite: the sprite to be drawn.
	// @param pos: the top-left position of the resulting drawn rectangle.
	void draw(const Sprite& sprite, const ssm::vec2& pos);

	// Issue a draw call, and clear all vertex and index buffers.
	void flush();

	// Issue a draw call, but keep vertices in memory.
	// Useful for drawing the same stuff multiple times.
	void issue_draw_call();

private:
	gl::VertexArrayObject vao;
	gl::BufferObject vert_buffer, index_buffer;
	Resource<Texture> batch_texture;

	gl::BufferStream<SpriteVertex> vertices;
	gl::BufferStream<size_type> indices;
};
