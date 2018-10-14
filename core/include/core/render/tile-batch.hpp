#pragma once

#include <glad/glad.h>
#include <ssm/vector.hpp>

#include "glinterface/common.hpp"
#include "glinterface/stream-buffer.hpp"

#include "core/resource/resource-cache.hpp"

#include "texture.hpp"
#include "tiles.hpp"

namespace render {
class TileBatch
{
public:
	TileBatch(ResourceCache<Texture>& resources, const TileMap& map, const TileSet& set);
	
	void issue_draw_call();

private:
	void setup_vertex_format();
	void rebuild_positions();
	void rebuild_uvs();

	const TileMap& map;
	const TileSet& set;
	Resource<Texture> texture;
	gl::VertexArrayObject vao;
	// Store vertex positions and uvs separately, as only uvs
	// need to be updated frequently
	gl::BufferObject pos_buffer, uv_buffer, index_buffer;
	gl::BufferStream<TexCoord> uvs;
};
}
