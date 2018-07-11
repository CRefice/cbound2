#pragma once

#include "glinterface/globject.hpp"
#include "glinterface/glvertexarray.hpp"

#include "core/common/tilemap.hpp"
#include "core/resource/resource.hpp"
#include "shader.hpp"

class StrictTileMapRenderer
{
public:
	StrictTileMapRenderer(Resource<Shader> shader) : shader(shader) {}
	StrictTileMapRenderer(Resource<Shader> shader, Resource<TileMap> tilemap, Resource<TileSet> tileset) : shader(shader) {
		reset(tilemap, tileset);
	}

	void reset(Resource<TileMap> tilemap, Resource<TileSet> tileset);

	void draw();

private:
	Resource<Shader> shader;
	Resource<TileMap> tilemap;
	Resource<TileSet> tileset;
	GlObject<GlVertexArray> vao;
};
