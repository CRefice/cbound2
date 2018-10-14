#include <vector>

#include "render/tile-batch.hpp"

static const GLbitfield mapflags = GL_MAP_WRITE_BIT;

namespace render {
TileBatch::TileBatch(ResourceCache<Texture>& resources, const TileMap& map, const TileSet& set) : map(map), set(set),
	texture(resources.load(set.texture_id)),
	uvs(GL_ARRAY_BUFFER, map.size.x * map.size.y * 4) {
	setup_vertex_format();
	rebuild_positions();
	rebuild_uvs();
}

void TileBatch::issue_draw_call() {
	if (uvs.size() == 0)
		return;

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, uv_buffer);
	uvs.unmap();

	glBindTexture(GL_TEXTURE_2D, texture->handle());
	glDrawElements(GL_TRIANGLES, uvs.size(), GL_UNSIGNED_SHORT, reinterpret_cast<void*>(0));

	uvs.map(mapflags);
}

void TileBatch::setup_vertex_format() {
	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, pos_buffer);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE,
			sizeof(ssm::vec2), (void*)(0));

	glBindBuffer(GL_ARRAY_BUFFER, uv_buffer);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_UNSIGNED_SHORT, GL_TRUE,
			sizeof(TexCoord), (void*)(0));
	glBindVertexArray(0);
}

void TileBatch::rebuild_positions() {
	// Initialize vertex data
	std::vector<ssm::vec2> positions;
	std::vector<GLushort> indices;
	for (TileMap::Layer l : map.layers) {
		for (int i = 0; i < l.tiles.size(); ++i) {
			int id = l.tiles[i];
			if (id < 0) continue;
			int x = i % map.size.x;
			int y = i / map.size.x;
			ssm::vec2 pos = ssm::vec2(x, y) * map.tile_size;
			Rectangle<float> bounds(pos, pos + map.tile_size);
			GLushort base_index = indices.size();
			positions.push_back(bounds.top_left());
			positions.push_back(bounds.bottom_left());
			positions.push_back(bounds.bottom_right());
			positions.push_back(bounds.top_right());
			indices.emplace_back(base_index);
			indices.emplace_back(base_index + 1);
			indices.emplace_back(base_index + 2);
			indices.emplace_back(base_index);
			indices.emplace_back(base_index + 2);
			indices.emplace_back(base_index + 3);
		}
	}
	glBindBuffer(GL_ARRAY_BUFFER, pos_buffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);
	glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(ssm::vec2), positions.data(), GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLushort), indices.data(), GL_STATIC_DRAW);
}

void TileBatch::rebuild_uvs() {
	glBindBuffer(GL_ARRAY_BUFFER, uv_buffer);
	glBufferData(GL_ARRAY_BUFFER, map.size.x * map.size.y * 4 * sizeof(TexCoord), nullptr, GL_DYNAMIC_DRAW);
	uvs.map(mapflags | GL_MAP_INVALIDATE_BUFFER_BIT);
	for (TileMap::Layer l : map.layers) {
		for (int i = 0; i < l.tiles.size(); ++i) {
			int id = l.tiles[i];
			if (id < 0) continue;
			TexFrame frame = frame_from_id(set, id);
			auto size = texture->size();
			uvs.push_back(normalize(frame.top_left(), size));
			uvs.push_back(normalize(frame.bottom_left(), size));
			uvs.push_back(normalize(frame.bottom_right(), size));
			uvs.push_back(normalize(frame.top_right(), size));
		}
	}
}
}
