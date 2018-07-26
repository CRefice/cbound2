#include "common/rectangle.hpp"

#include "render/sprite-batch.hpp"

static const GLbitfield mapflags = GL_MAP_WRITE_BIT
	| GL_MAP_INVALIDATE_BUFFER_BIT
	| GL_MAP_INVALIDATE_RANGE_BIT;

SpriteBatch::SpriteBatch(size_type batch_size) :
	batch_texture(nullptr, ""),
	vertices(GL_ARRAY_BUFFER, batch_size * 4),
	indices(GL_ELEMENT_ARRAY_BUFFER, batch_size * 6) {
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vert_buffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);

	// Initialize the vertex and element buffer data
	// For every sprite there are 4 verts and 6 indices.
	glBufferData(GL_ARRAY_BUFFER, batch_size * 4 * sizeof(SpriteVertex), nullptr, GL_STREAM_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, batch_size * 6 * sizeof(size_type), nullptr, GL_STREAM_DRAW);
	vertices.map(mapflags);
	indices.map(mapflags);

	// Vertex format:
	// 0: pos.x pos.y (floats)
	// 1: uv.x uv.y (in shorts, normalized)
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE,
			sizeof(SpriteVertex), reinterpret_cast<void*>(offsetof(SpriteVertex, pos)));

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_UNSIGNED_SHORT, GL_TRUE,
			sizeof(SpriteVertex), reinterpret_cast<void*>(offsetof(SpriteVertex, uv)));

	glBindVertexArray(0);
}

void SpriteBatch::draw(const Sprite& sprite, const ssm::vec2& pos) {
	if (sprite.texture != batch_texture) {
		flush();
		batch_texture = sprite.texture;
	}

	// Winding order: counterclockwise faces front-wards
	const Rectangle<float> bounds(pos, pos + sprite.size);
	const size_type base_index = vertices.size();
	vertices.emplace_back(bounds.top_left(), sprite.tex_coords.top_left());
	vertices.emplace_back(bounds.bottom_left(), sprite.tex_coords.bottom_left());
	vertices.emplace_back(bounds.bottom_right(), sprite.tex_coords.bottom_right());
	vertices.emplace_back(bounds.top_right(), sprite.tex_coords.top_right());

	indices.emplace_back(base_index);
	indices.emplace_back(base_index + 1);
	indices.emplace_back(base_index + 2);
	indices.emplace_back(base_index);
	indices.emplace_back(base_index + 2);
	indices.emplace_back(base_index + 3);
}

void SpriteBatch::flush() {
	issue_draw_call();
	vertices.clear();
	indices.clear();
}

void SpriteBatch::issue_draw_call() {
	if (indices.size() == 0)
		return;

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vert_buffer);
	vertices.unmap();
	indices.unmap();

	glBindTexture(GL_TEXTURE_2D, batch_texture->handle());
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_SHORT, reinterpret_cast<void*>(0));

	vertices.map(mapflags);
	indices.map(mapflags);
}
