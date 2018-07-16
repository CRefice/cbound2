#include "sprite-batch.hpp"

static const GLbitfield mapflags = GL_MAP_WRITE_BIT
	| GL_MAP_INVALIDATE_BUFFER_BIT
	| GL_MAP_INVALIDATE_RANGE_BIT;

SpriteBatch::SpriteBatch(size_type batch_size) :
	vertices(GL_ARRAY_BUFFER, batch_size * 4),
	indices(GL_ELEMENT_ARRAY_BUFFER, batch_size * 6) {
	// Generate buffers and bind them
	glGenVertexArrays(1, &vao);
	//We could really avoid doing this and call genBuffers twice, I just wanna know if it works
	static_assert(offsetof(SpriteBatch, index_buffer) == offsetof(SpriteBatch, vert_buffer) + sizeof(GLuint),
			"OpenGL buffers in SpriteBatch are not contiguous in memory");
	glGenBuffers(2, &vert_buffer);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vert_buffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);

	// Initialize the vertex and element buffer data
	// For every sprite there are 4 verts and 6 indices.
	glBufferData(GL_ARRAY_BUFFER, batch_size * 4, nullptr, GL_STREAM_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, batch_size * 6, nullptr, GL_STREAM_DRAW);
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

SpriteBatch::~SpriteBatch() {
	glDeleteBuffers(2, &vert_buffer);
}

// We add the vertices and indices directly to the buffer
// by mapping it and writing into the mapped pointer.
void SpriteBatch::draw(const Sprite& sprite, const ssm::vec2& pos) {
	//if (sprite.texture != batch_texture) {
	//	flush();
	//	batch_texture = sprite.texture;
	//}

	auto left = sprite.texcoords.top_left.x;
	auto right = sprite.texcoords.bottom_right.x;
	auto top = sprite.texcoords.top_left.y;
	auto bottom = sprite.texcoords.bottom_right.y;
	ssm::vec2 size = { right - left, top - bottom };

	// Counterclockwise faces front-wards
	// Winding order: top left, bottom left, bottom right, top right
	const size_type base_index = vertices.size() * 4;
	vertices.emplace_back( pos, sprite.texcoords.top_left );
	vertices.emplace_back( pos + ssm::vec2(0, size.y), ssm::vector<GLushort, 2>(left, bottom));
	vertices.emplace_back( pos + size, sprite.texcoords.bottom_right );
	vertices.emplace_back( pos + ssm::vec2(size.x, 0), ssm::vector<GLushort, 2>(right, top));

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
	vertices.unmap();
	indices.unmap();
	glBindVertexArray(vao);

	glDrawElements(GL_TRIANGLES, count * 6, GL_UNSIGNED_SHORT, reinterpret_cast<void*>(0));

	vertices.map(mapflags);
	indices.map(mapflags);
}
