#pragma once

#include <cstdint>

#include <glad/glad.h>
#include <ssm/vector.hpp>

#include "glinterface/common.hpp"
#include "glinterface/stream-buffer.hpp"

#include "core/anim/sequencer.hpp"
#include "core/resource/resource-cache.hpp"

#include "tex-coords.hpp"
#include "texture.hpp"
#include "tiles.hpp"

namespace render {
// A static collection of tiles, to be drawn multiple times.
class StaticTileBatch
{
public:
	StaticTileBatch(ResourceCache<Texture>& resources, const TileMap& map, const TileSet& set);
	
	// Draw the tile set, keeping all information in memory.
	void issue_draw_call();

private:
	Resource<Texture> texture;
	gl::VertexArrayObject vao;
	std::size_t index_count;
};

// Draws animated tiles
class AnimTileBatch
{
public:
	AnimTileBatch(ResourceCache<Texture>& resources, const TileMap& map, const TileSet& set);
	
	// Advance the animation counter for all tiles by dt seconds.
	void progress(double dt);

	// Draw the tiles at the current animation frame.
	void issue_draw_call();

private:
	Resource<Texture> texture;
	std::unordered_map<int, anim::Sequencer<int>> sequencers;
	// The tile id of each animated tile on the map
	std::vector<int> ids;
	gl::VertexArrayObject vao;
	gl::BufferObject pos_buf, uv_buf, ind_buf;
	// Uvs are the only thing going to be updated
	gl::BufferStream<TexCoord> uvs;
	const TileMap& map;
	const TileSet& set;
};
}
